#include "motor/ctrl_foc.h"
#include "main.h"
#include "motor/trigonometric.h"
#include "analog/adc1/main.h"

Result motor_foc_tim_setup(const MotorParameter *motor)
{
    HAL_TIM_PWM_Start(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[0]);
    HAL_TIM_PWM_Start(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[1]);
    HAL_TIM_PWM_Start(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[2]);
    HAL_TIMEx_PWMN_Start(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[0]);
    HAL_TIMEx_PWMN_Start(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[1]);
    HAL_TIMEx_PWMN_Start(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[2]);
    return RESULT_OK(NULL);
}

// Thread - hallExti - 0
Result motor_foc_hall_update(MotorParameter *motor)
{
    float htim_cnt = (float)__HAL_TIM_GET_COUNTER(motor->const_h.ELE_htimx);
    __HAL_TIM_SET_COUNTER(motor->const_h.ELE_htimx, 0);
    motor->rpm_fbk_hall = 100000000.0f / htim_cnt;

    uint16_t expected = (!motor->reverse)
        ? hall_seq_clw[motor->exti_hall_last]
        : hall_seq_ccw[motor->exti_hall_last];
    // if (hall_last == 0) // ? CHECK
    // {
    //     hall_last = expected;
    // }
    if (motor->exti_hall_curt == expected)
    {
        // rotated
        motor->hall_angle_acc = 0;
        motor->pwm_it_angle_acc = 0;
    }

    // ? check
    // 電氣週期算轉速，分鐘[3G=50,000,000 (計數轉秒)*60(秒轉分鐘)] / 轉速
    // calculate speed every hall instead of  6 times
    // agv gear ratio MOTOR_42BLF01_GEAR
    motor->pi_speed.Fbk = (6000000.0f / (htim_cnt * (MOTOR_42BLF01_POLE / 2))) / 6 / MOTOR_42BLF01_GEAR;
    // 單次PWM中斷時的角度變化 50us*60/(0.1us*CNT)
    motor->pwm_per_it_angle_itpl = 30000.0f * DEG_TO_RAD / htim_cnt;

    return RESULT_OK(NULL);
}

// Thread - pwmIt - 1
static inline Result stop_check(MotorParameter *motor)
{
    // 停轉判斷
    // 現在與上一個霍爾的總和與之前的總和相同，視為馬達靜止不動
    uint8_t hall_current = motor->exti_hall_curt;
    uint16_t hall_total = motor->pwm_hall_last*10 + hall_current;
    if(hall_total == motor->pwm_hall_acc)
    {
        motor->spin_stop_acc++;
        if (motor->spin_stop_acc >= 3000)
        {
            motor->spin_stop_acc = 0;
            // timerclockvalue_onecycle_electric = 0;   // 歸零一電氣週期之時間
            motor->pi_speed.i1 = 0;                     // 重置i控制舊值
            motor->pi_speed.Fbk = 0;                    // 歸零速度實際值
            motor->pi_Iq.Out=0;
        }
    }
    else
    {
        motor->spin_stop_acc = 0;
    }
    motor->pwm_hall_acc = hall_total;
    motor->pwm_hall_last = hall_current;
    return RESULT_OK(NULL);
}

static inline Result pi_speed(MotorParameter *motor)
{
    // 計算 速度PI (每100個PWM中斷)
    // if(Speed.Fbk>0 && stop_flag==0)
    PI_run(&motor->pi_speed);
    motor->pi_speed_cmd = CLAMP((motor->pi_speed_cmd + motor->pi_speed.Out), 0.2, 0.15);
    // else if(Speed.Fbk==0 | stop_flag==1)
    //     motor->pi_speed_cmd=0.18;
    return RESULT_OK(NULL);
}

// Thread - pwmIt - 3
static const float adc_to_current = (3.3f / 4095.0f) / 0.185f; // ~ 0.004356 A/LSB
static inline Result vec_ctrl_clarke(MotorParameter *motor)
{
    // clarke ideal
    // I alpha = Ia
    // I bata = (Ia+2Ib)/根號3
    // clarke nonideal
    // I alpha = 2/3Ia - 1/3Ib - 1/3Ic
    // I bata = (根號3/3)Ib - (根號3/3)Ic
    // (根號3/3) = 0.57735

    // 三相電流向量
    float adc_zero = (float)(motor->adc_u + motor->adc_v + motor->adc_w) / 3 ;
    motor->clarke.As = ((float)motor->adc_u - adc_zero) * adc_to_current;
    motor->clarke.Bs = ((float)motor->adc_v - adc_zero) * adc_to_current;
    motor->clarke.Cs = ((float)motor->adc_w - adc_zero) * adc_to_current;

    // 數位濾波
    // PeriodStateVar_u += ( ( (float)motor->clarke.As - (float)PeriodFilter_u)*(float)PeriodKFilter );
    // PeriodFilter_u = (float)PeriodStateVar_u;//0.9
    // motor->clarke.As =PeriodFilter_u;
    //		
    // PeriodStateVar_v += ( ( (float)motor->clarke.Bs - (float)PeriodFilter_v)*(float)PeriodKFilter );
    // PeriodFilter_v = (float)PeriodStateVar_v;//0.9
    // motor->clarke.Bs =PeriodFilter_v;

    // PeriodStateVar_w += ( ( (float)motor->clarke.Cs - (float)PeriodFilter_w)*(float)PeriodKFilter );
    // PeriodFilter_w = (float)PeriodStateVar_w;//0.9
    // motor->clarke.Cs =PeriodFilter_w;
		
    CLARKE_run_ideal(&motor->clarke);//Id.Out=CLAMP((Id.Out + Id.delta), 0.1, 0)

    return RESULT_OK(NULL);
}

// Thread - pwmIt - 4
static inline Result vec_ctrl_park(MotorParameter *motor)
{
    motor->pwm_it_angle_acc += motor->pwm_per_it_angle_itpl;
    float foc_cal_rad;
    motor_hall_to_angle(motor->exti_hall_curt, &foc_cal_rad);
    foc_cal_rad += motor->pwm_it_angle_acc;

    if      (foc_cal_rad >= MUL_2_PI) foc_cal_rad -= MUL_2_PI;
    else if (foc_cal_rad <      0.0f) foc_cal_rad += MUL_2_PI;
    foc_cal_rad += DIV_PI_2 * 3;

    // park
    // Id = I alpha cos(theta) + I bata sin(theta)
    motor->park.Alpha = motor->clarke.Alpha;
    motor->park.Beta = motor->clarke.Beta;

    motor->park.Sine = TableSearch_sin(foc_cal_rad);
    motor->park.Cosine = TableSearch_sin(foc_cal_rad + DIV_PI_2);
    
    PARK_run(&motor->park);
    
    // PARK_MACRO_Swap(motor->park);
    
    // PeriodStateVar_id += ( ( (float)motor->park.Ds - (float)PeriodFilter_id)*(float)PeriodKFilter );
    // PeriodFilter_id = (float)PeriodStateVar_id;
    // motor->park.Ds = PeriodFilter_id;
        
    // PeriodStateVar_iq += ( ( (float)motor->park.Qs - (float)PeriodFilter_iq)*(float)PeriodKFilter );
    // PeriodFilter_iq = (float)PeriodStateVar_iq;
    // motor->park.Qs = PeriodFilter_iq;
    return RESULT_OK(NULL);
}

// Thread - pwmIt - 5
#define IQ_REF_ADD 0
static inline Result vec_ctrl_pi_id_iq(MotorParameter *motor)
{
    // Id、Iq 之 PI 控制
    if(motor->pi_speed.Fbk > 0)
    {
        motor->pi_Id.Ref = 0;
        motor->pi_Id.Fbk = motor->park.Ds;
        
        // Id.Fbk=CLAMP(( motor->park.Ds), 0.1, -0.1);
        
        PI_run(&motor->pi_Id); 
        
        // if(Id.Ref > Id.Fbk)
        //     Id.delta = 0.002;
        // else
        //     Id.delta = -0.002;
        
        motor->pi_Id.Out = CLAMP((motor->pi_Id.Out), 0.01, -0.01);//限制最大與最小參數
        
        // Id.Out=CLAMP((Id.Out + Id.delta), 0.08, -0.08);//限制最大與最小參數
        
        // Speed.delta = Speed.Ref - Speed.Fbk;
        // Iq.Ref = 0.6 + (Speed.delta * Speed.Kp);//motor->pi_speed_cmd * Current_base

        // motor->pi_Iq.Ref = motor->pi_speed_cmd + IQ_REF_ADD;
        // motor->pi_Iq.Fbk = motor->park.Qs;
        // motor->pi_Iq.delta = (motor->pi_Iq.Ref - motor->pi_Iq.Fbk) * (motor->pi_Iq.Kp);
        // motor->pi_Iq.delta = CLAMP((motor->pi_Iq.delta), 0.1, -0.1);//限制最大與最小參數
        // motor->pi_Iq.Out = CLAMP((motor->pi_Iq.Ref + motor->pi_Iq.delta), 0.75, 0);//限制最大與最小參數

        motor->pi_Iq.Ref = motor->pi_speed_cmd + IQ_REF_ADD;  // 外環給轉矩命令
        motor->pi_Iq.Fbk = motor->park.Qs;                    // q 軸量測
        PI_run(&motor->pi_Iq);                                // 統一用 PI_run + anti-windup
        // 視匯流排/過調制上限，做一次幅值限幅（可留在這或放到 Vqref 指派前）
        motor->pi_Iq.Out = CLAMP(motor->pi_Iq.Out, 0.75f, 0.0f);

    }
    else
    {
        motor->pi_Iq.Out = 0.18;
    }
    return RESULT_OK(NULL);
}

// Thread - pwmIt - 6
static inline Result vec_ctrl_ipark(MotorParameter *motor)
{
    // ipark
    // V alpha = Vd cos(theta) - Vq sin(theta)
    // V bata = Vd sin(theta) + Vq cos(theta)

    //限制最大與最小參數
    // ? motor->ipark.Vdref = CLAMP((motor->ipark.Vdref += motor->pi_Id.Out), 0.06, -0.06);
    motor->ipark.Vdref = CLAMP(motor->pi_Id.Out, 0.06, -0.06);

    motor->ipark.Vqref = motor->pi_Iq.Out;

    motor->ipark.Sine = motor->park.Sine;
    motor->ipark.Cosine = motor->park.Cosine;

    IPARK_run(&motor->ipark);
    return RESULT_OK(NULL);
}

static inline Result vec_ctrl_svgen(MotorParameter *motor)
{
    // svgen  //5us
    motor->svgendq.Ualpha = motor->ipark.Alpha;
    motor->svgendq.Ubeta = motor->ipark.Beta;
    SVGEN_run(&motor->svgendq);

    motor->elec_theta_rad = TableSearch_atan2(motor->ipark.Beta, motor->ipark.Alpha);
    while (motor->elec_theta_rad < 0.0f) motor->elec_theta_rad += MUL_2_PI;

    float elec_theta_deg = motor->elec_theta_rad * RAD_TO_DEG;
    while (elec_theta_deg >= 360.0f) elec_theta_deg -= 360.0f;
    while (elec_theta_deg <    0.0f) elec_theta_deg += 360.0f;
    motor->elec_theta_deg = elec_theta_deg;

    return RESULT_OK(NULL);
}

static inline Result vec_ctrl_vref(MotorParameter *motor)
{
    // --------------------------------------------------------------------------
    // 6us
    // motor->svpwm_Vref = sqrt(motor->svgendq.Ualpha*motor->svgendq.Ualpha + motor->svgendq.Ubeta * motor->svgendq.Ubeta);
    
    // if(stop_flag==1)
    //     {
    //     if(motor->svpwm_Vref>0.0001)
    //     {
    //         motor->svpwm_Vref-=0.0001;
    //         Iq.Out=0.18 ;
    //     }
    //     else
    //         motor->svpwm_Vref=0;
    //     }
    // else
        float Vref = sqrt(
            motor->svgendq.Ualpha * motor->svgendq.Ualpha 
            + motor->svgendq.Ubeta * motor->svgendq.Ubeta);
        motor->svpwm_Vref = Vref;

    // motor->svpwm_Vref = Iq.Out;
    
    // ------------------------------------
    //		motor_angle = motor_angle + deg_add;	
    //		if(motor_angle < 0)
    //			motor_angle = motor_angle + 360;
    //		
    //		svpwm_interval = ((int)motor_angle / 60) % 6;
    //		motor->elec_theta_deg      =  (int)motor_angle % 60;
    
    /*
        svpwm_interval = ((int)cmd_deg / 60) % 6;
        motor->elec_theta_deg      =  (int)cmd_deg % 60;*/
    return RESULT_OK(NULL);
}

// Thread - pwmIt - 7
static inline Result vec_ctrl_svpwm(MotorParameter *motor)
{
    float T1, T2;
    float theta_in_sector = motor->elec_theta_rad;
    while (theta_in_sector >= DIV_PI_3) theta_in_sector -= DIV_PI_3;
    // ? CHECK
    if(!motor->reverse)
    {
        T1 = motor->svpwm_Vref * TableSearch_sin(DIV_PI_3 - theta_in_sector);
        T2 = motor->svpwm_Vref * TableSearch_sin(theta_in_sector);
    }
    else
    {
        T1 = motor->svpwm_Vref * TableSearch_sin(theta_in_sector);
        T2 = motor->svpwm_Vref * TableSearch_sin(DIV_PI_3 - theta_in_sector);
    }

    //------------------------------------
    float T0div2 = (1 - (T1 + T2)) / 2;

    // switch(motor->svgendq.Sector)
    // // switch(my_hall_signal_in_decimal)
    // {
    //     case 3://120~179 2 acb
    //     {
    //         motor->pwm_duty_u = T0div2 + T1 + T2;
    //         motor->pwm_duty_v = T0div2;
    //         motor->pwm_duty_w = T0div2 + T1;
    //         // Ta = T0div2;
    //         // Tb = T0div2+T1+T2;
    //         // Tc = T0div2+T2;
    //         break;
    //     }
    //     case 1://180~239 3 abc
    //     {
    //         motor->pwm_duty_u = T0div2 + T1 + T2;
    //         motor->pwm_duty_v = T0div2 + T2;
    //         motor->pwm_duty_w = T0div2;
    //         // Ta = T0div2;
    //         // Tb = T0div2+T1;
    //         // Tc = T0div2+T1+T2;
    //         break;
    //     }
    //     case 5://240~299 1    bac
    //     {
    //         motor->pwm_duty_u = T0div2 + T1;
    //         motor->pwm_duty_v = T0div2 + T1 + T2;
    //         motor->pwm_duty_w = T0div2;
    //         // Ta = T0div2+T2;
    //         // Tb = T0div2;
    //         // Tc = T0div2+T1+T2;
    //         break;
    //     }
    //     case 4://300~359 5  bca
    //     {
    //         motor->pwm_duty_u = T0div2;
    //         motor->pwm_duty_v = T0div2 + T1 + T2;
    //         motor->pwm_duty_w = T0div2 + T2;
    //         // Ta = T0div2+T1+T2;
    //         // Tb = T0div2;
    //         // Tc = T0div2+T1;
    //         break;
    //     }
    //     case 6://0~59 4 cba
    //     {
    //         motor->pwm_duty_u = T0div2;
    //         motor->pwm_duty_v = T0div2 + T1;
    //         motor->pwm_duty_w = T0div2 + T1 + T2;
    //         // Ta = T0div2+T1+T2;
    //         // Tb = T0div2+T2;
    //         // Tc = T0div2;
    //         break;
    //     }
    //     case 2://60~119 6 cba
    //     {
    //         motor->pwm_duty_u = T0div2 + T2;
    //         motor->pwm_duty_v = T0div2;
    //         motor->pwm_duty_w = T0div2 + T1 + T2;
    //         // Ta = T0div2+T1;
    //         // Tb = T0div2+T1+T2;
    //         // Tc = T0div2;
    //         break;
    //     }
    // }
    // __HAL_TIM_SET_COMPARE(motor->const_h.htimx[0], motor->const_h.TIM_CHANNEL_x[0], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_u));
    // __HAL_TIM_SET_COMPARE(motor->const_h.htimx[1], motor->const_h.TIM_CHANNEL_x[1], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_v));
    // __HAL_TIM_SET_COMPARE(motor->const_h.htimx[2], motor->const_h.TIM_CHANNEL_x[2], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_w));
    return RESULT_OK(NULL);
}

// Thread - pwmIt - ex
static UNUSED_FNC inline Result data_ret(MotorParameter *motor)
{
	//--------------------------------------------------------------------------
    /*
	if(glo_SaveToArray_flag == 1)
	{
		if(SaveToArray_count < 600)
		{
            // SaveToArray_v0[SaveToArray_count] = motor->clarke.As;
            // SaveToArray_v1[SaveToArray_count] = motor->clarke.Bs;
            // SaveToArray_v2[SaveToArray_count] = motor->clarke.Cs;
            // SaveToArray_v3[SaveToArray_count] = PeriodFilter_u;
            // SaveToArray_v4[SaveToArray_count] = sg_deg;
            SaveToArray_v3[SaveToArray_count] = test_deg;
            SaveToArray_v4[SaveToArray_count] = my_hall_signal_in_decimal;
			
            // SaveToArray_v6[SaveToArray_count] = svpwm_interval;
            // SaveToArray_v7[SaveToArray_count] = motor->svgendq.Sector;

            // SaveToArray_v4[SaveToArray_count] = motor->park.Alpha;
            // SaveToArray_v5[SaveToArray_count] = motor->park.Beta;

            // SaveToArray_v6[SaveToArray_count] = Id.Fbk;
            // SaveToArray_v7[SaveToArray_count] = Iq.Fbk;
            // SaveToArray_v8[SaveToArray_count] = motor->park.Vdref;
            // SaveToArray_v9[SaveToArray_count] = motor->park.Vqref;
            //
            // SaveToArray_v7[SaveToArray_count] = test_deg;

            SaveToArray_v0[SaveToArray_count] = Ta;
            SaveToArray_v1[SaveToArray_count] = Tb;
            SaveToArray_v2[SaveToArray_count] = Tc;

            SaveToArray_count++; 
        }
        else
        {
            SaveToArray_count=0;
            glo_SaveToArray_flag = 2;
        }
    }
    */
    return RESULT_OK(NULL);
}//FOC 計算 END

// FOC 20kHz
// Thread - pwmIt - 0
Result motor_foc_pwm_pulse(MotorParameter *motor)
{
    motor->pwm_count++;
    if (motor->pwm_count % 2 == 0)
    {
        // ?
        if((motor->hall_angle_acc + motor->pwm_per_it_angle_itpl) < 60)
        {
            motor->hall_angle_acc += motor->pwm_per_it_angle_itpl;
            motor->hall_angle_acc = CLAMP(motor->hall_angle_acc , 60, 0);
        }
        renew_adc(motor->const_h.adc_u_id, &motor->adc_u);
        renew_adc(motor->const_h.adc_v_id, &motor->adc_v);
        renew_adc(motor->const_h.adc_w_id, &motor->adc_w);
        vec_ctrl_clarke(motor);
        vec_ctrl_park(motor);
        vec_ctrl_pi_id_iq(motor);
        vec_ctrl_ipark(motor);
        vec_ctrl_svgen(motor);
        vec_ctrl_vref(motor);
        vec_ctrl_svpwm(motor);
    }
    if (motor->pwm_count % 100 == 0)
    {
        stop_check(motor);
        pi_speed(motor);
    }
    if (motor->pwm_count >= 1000)
    {
        motor->rpm_fbk_htim = motor->exti_hall_cnt * 200.0f;
        motor->exti_hall_cnt = 0;
        motor->pwm_count = 0;
    }
    return RESULT_OK(NULL);
}
