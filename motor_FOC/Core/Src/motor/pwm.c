#include "motor/pwm.h"
#include "motor/trigonometric.h"

static void pwm_setup(const MotorParameter *motor)
{
    const MotorConst* const_h = &motor->const_h;
    HAL_TIM_PWM_Start(const_h->PWM_htimx[0], const_h->PWM_TIM_CHANNEL_x[0]);
    HAL_TIM_PWM_Start(const_h->PWM_htimx[1], const_h->PWM_TIM_CHANNEL_x[1]);
    HAL_TIM_PWM_Start(const_h->PWM_htimx[2], const_h->PWM_TIM_CHANNEL_x[2]);
}

static uint16_t hall_to_angle(uint8_t value)
{
    switch(value)
    {
        case 6:	return 0;
        case 2: return 60;
        case 3:	return 120;
        case 1:	return 180;
        case 5:	return 240;
        case 4:	return 300;
        default:
        {
            Error_Handler();
            break;
        }
    }
}

#define FOC_CAL_DEG_ADD 270
#define DEG_TO_RAD 0.01745329f
#define RAD_TO_DEG 57.2957f
#define IQ_REF_ADD 0
static inline void Vector_control_calculate(MotorParameter *motor)
{
    float motor_angle = (hall_to_angle(motor->gpio_hall_current) + (uint16_t)motor->pwm_it_angle_acc) % 360;

    float foc_cal_deg = motor_angle + FOC_CAL_DEG_ADD;
    // --------------------------------------------------------------------------
    // clarke(理想)
    // I alpha = Ia
    // I bata = (Ia+2Ib)/根號3

    // clarke(非理想)
    // I alpha = 2/3Ia - 1/3Ib - 1/3Ic
    // I bata = (根號3/3)Ib - (根號3/3)Ic
    // (根號3/3) = 0.57735

    // 三相電流向量
    // motor->clarke.As = ((float)glo_ADC0_value[0]-2300)/250;// /50*5
    // motor->clarke.Bs = ((float)glo_ADC0_value[1]-2322)/250;
    // motor->clarke.Cs = ((float)glo_ADC0_value[2]-2300)/250;
	
    float adc_zero = (float)(motor->adc_u + motor->adc_v + motor->adc_w) / 3 ;
    
    motor->clarke.As = ((float)motor->adc_u - adc_zero) / 1200; // 1200
    motor->clarke.Bs = ((float)motor->adc_v - adc_zero) / 1200;
    motor->clarke.Cs = ((float)motor->adc_w - adc_zero) / 1200;


    //    1/4095*3.3*1000/185 = 0.004356
    //  / 4095 * 3.3 to change from adc back to voltage
    // *1000/185 , is to change from V to mV
    // and acs712 is 1A for every 185 mv
    // so 1/4095*3.3*1000/185 will change from adc4095 to current in Amps

////const float my_adc_4095_to_current = 0.004356 ;

////		motor->clarke.As = ((float)glo_ADC0_value[0]-adc_zero)*my_adc_4095_to_current;// 1200
////		motor->clarke.Bs = ((float)glo_ADC0_value[1]-adc_zero)*my_adc_4095_to_current;
////		motor->clarke.Cs = ((float)glo_ADC0_value[2]-adc_zero)*my_adc_4095_to_current;	

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
		
    CLARKE_MACRO(motor->clarke);//Id.Out=CLAMP((Id.Out + Id.delta), 0.1, 0)
    // --------------------------------------------------------------------------
    // park
    // Id = I alpha cos(theta) + I bata sin(theta)
    motor->park.Alpha = motor->clarke.Alpha;
    motor->park.Beta = motor->clarke.Beta;

    motor->park.Sine = foc_cal_deg * DEG_TO_RAD;
    TableSearch_sin(motor->park.Sine,motor->park.Sine);
    
    motor->park.Cosine = (foc_cal_deg+90) * DEG_TO_RAD;
    TableSearch_sin(motor->park.Cosine,motor->park.Cosine);
    
    PARK_MACRO(motor->park);
    
    // PARK_MACRO_Swap(motor->park);
    
    // PeriodStateVar_id += ( ( (float)motor->park.Ds - (float)PeriodFilter_id)*(float)PeriodKFilter );
    // PeriodFilter_id = (float)PeriodStateVar_id;
    // motor->park.Ds = PeriodFilter_id;
        
    // PeriodStateVar_iq += ( ( (float)motor->park.Qs - (float)PeriodFilter_iq)*(float)PeriodKFilter );
    // PeriodFilter_iq = (float)PeriodStateVar_iq;
    // motor->park.Qs = PeriodFilter_iq;
    // --------------------------------------------------------------------------	
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

        motor->pi_Iq.Ref = motor->pi_speed_cmd + IQ_REF_ADD;

        motor->pi_Iq.Fbk = motor->park.Qs;
        motor->pi_Iq.delta = (motor->pi_Iq.Ref - motor->pi_Iq.Fbk) * (motor->pi_Iq.Kp);

        motor->pi_Iq.delta = CLAMP((motor->pi_Iq.delta), 0.1, -0.1);//限制最大與最小參數
        
        motor->pi_Iq.Out = CLAMP((motor->pi_Iq.Ref + motor->pi_Iq.delta), 0.75, 0);//限制最大與最小參數
    }
    else
    {
        motor->pi_Iq.Out = 0.18;
    }
    // PI_MACRO(Id);
    // PI_MACRO(Iq);
    // --------------------------------------------------------------------------
    // 反park
    // V alpha = Vd cos(theta) - Vq sin(theta)
    // V bata = Vd sin(theta) + Vq cos(theta)

    // motor->park.Vdref = Id.Out;// + PId.Out
    motor->ipark.Vdref = CLAMP((motor->ipark.Vdref += motor->pi_Id.Out), 0.06, -0.06);//限制最大與最小參數

    motor->ipark.Vqref = motor->pi_Iq.Out;

    // motor->ipark.Vdref = Id.Ref +Id.Out; //motor->ipark.Vdref = 0;//測試用
    // motor->ipark.Vqref = Iq.Ref +Iq.Out;

    motor->ipark.Sine = motor->park.Sine;
    motor->ipark.Cosine = motor->park.Cosine;

    IPARK_MACRO(motor->ipark);
    // --------------------------------------------------------------------------
    // svgen  //5us

    motor->svgendq.Ualpha = motor->ipark.Alpha;
    motor->svgendq.Ubeta = motor->ipark.Beta;
    SVGEN_MACRO(motor->svgendq);

    motor->electric_theta_rad = motor->ipark.Beta / motor->ipark.Alpha;

    TableSearch_atan(motor->electric_theta_rad, motor->electric_theta_rad); // ? How to work

    // --------------------------------------------------------------------------
    // 6us
    // motor->SVPWM_Vref = sqrt(motor->svgendq.Ualpha*motor->svgendq.Ualpha + motor->svgendq.Ubeta * motor->svgendq.Ubeta);
    
    // if(stop_flag==1)
    //     {
    //     if(motor->SVPWM_Vref>0.0001)
    //     {
    //         motor->SVPWM_Vref-=0.0001;
    //         Iq.Out=0.18 ;
    //     }
    //     else
    //         motor->SVPWM_Vref=0;
    //     }
    // else
        motor->SVPWM_Vref = sqrt(motor->svgendq.Ualpha * motor->svgendq.Ualpha + motor->svgendq.Ubeta * motor->svgendq.Ubeta);
        
    // motor->SVPWM_Vref = Iq.Out;
    
    // ------------------------------------
    //		motor_angle = motor_angle + deg_add;	
    //		if(motor_angle < 0)
    //			motor_angle = motor_angle + 360;
    //		
    //		svpwm_interval = ((int)motor_angle / 60) % 6;
    //		motor->electric_theta_deg      =  (int)motor_angle % 60;

    
    // sg_deg = sg_deg + deg_add  +6 ;	
    
    float sg_deg = motor->electric_theta_rad * RAD_TO_DEG;
    
    if(sg_deg < 0) sg_deg = sg_deg + 360;
    
    motor->electric_theta_deg = (uint32_t)sg_deg % 60;
    
    // motor->electric_theta_deg -= 6 ; 
    // adding here is wrong
    
    /*
        svpwm_interval = ((int)cmd_deg / 60) % 6;
        motor->electric_theta_deg      =  (int)cmd_deg % 60;*/
    //------------------------------------
    float T1, T2;
    if(!motor->reverse) // ? CHECK
    {
        T1 = (60 - motor->electric_theta_deg) * DEG_TO_RAD;
        TableSearch_sin(T1, T1);
        T1 = T1 * motor->SVPWM_Vref;
        //------------------------------------
        T2 = (motor->electric_theta_deg) * DEG_TO_RAD;
        TableSearch_sin(T2, T2);
        T2 = T2 * motor->SVPWM_Vref;
    }
    else
    {
        T2 = (60 - motor->electric_theta_deg) * DEG_TO_RAD;
        TableSearch_sin(T2, T2);
        T2 = T2 * motor->SVPWM_Vref;
        //------------------------------------
        T1 = (motor->electric_theta_deg) * DEG_TO_RAD;
        TableSearch_sin(T1, T1);
        T1 = T1 * motor->SVPWM_Vref;
    }

    //------------------------------------
    float T0div2 = (1 - (T1 + T2)) / 2;

    const MotorConst* const_h = &motor->const_h;
    switch(motor->svgendq.Sector)
    // switch(my_hall_signal_in_decimal)
    {
        case 3://120~179 2 acb
        {
            motor->pwm_duty_u = T0div2 + T1 + T2;
            motor->pwm_duty_v = T0div2;
            motor->pwm_duty_w = T0div2 + T1;
            // Ta = T0div2;
            // Tb = T0div2+T1+T2;
            // Tc = T0div2+T2;
            break;
        }
        case 1://180~239 3 abc
        {
            motor->pwm_duty_u = T0div2 + T1 + T2;
            motor->pwm_duty_v = T0div2 + T2;
            motor->pwm_duty_w = T0div2;
            // Ta = T0div2;
            // Tb = T0div2+T1;
            // Tc = T0div2+T1+T2;
            break;
        }
        case 5://240~299 1    bac
        {
            motor->pwm_duty_u = T0div2 + T1;
            motor->pwm_duty_v = T0div2 + T1 + T2;
            motor->pwm_duty_w = T0div2;
            // Ta = T0div2+T2;
            // Tb = T0div2;
            // Tc = T0div2+T1+T2;
            break;
        }
        case 4://300~359 5  bca
        {
            motor->pwm_duty_u = T0div2;
            motor->pwm_duty_v = T0div2 + T1 + T2;
            motor->pwm_duty_w = T0div2 + T2;
            // Ta = T0div2+T1+T2;
            // Tb = T0div2;
            // Tc = T0div2+T1;
            break;
        }
        case 6://0~59 4 cba
        {
            motor->pwm_duty_u = T0div2;
            motor->pwm_duty_v = T0div2 + T1;
            motor->pwm_duty_w = T0div2 + T1 + T2;
            // Ta = T0div2+T1+T2;
            // Tb = T0div2+T2;
            // Tc = T0div2;
            break;
        }
        case 2://60~119 6 cba
        {
            motor->pwm_duty_u = T0div2 + T2;
            motor->pwm_duty_v = T0div2;
            motor->pwm_duty_w = T0div2 + T1 + T2;
            // Ta = T0div2+T1;
            // Tb = T0div2+T1+T2;
            // Tc = T0div2;
            break;
        }
    }
    __HAL_TIM_SET_COMPARE(const_h->PWM_htimx[0], const_h->PWM_TIM_CHANNEL_x[0], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_u));
    __HAL_TIM_SET_COMPARE(const_h->PWM_htimx[1], const_h->PWM_TIM_CHANNEL_x[1], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_v));
    __HAL_TIM_SET_COMPARE(const_h->PWM_htimx[2], const_h->PWM_TIM_CHANNEL_x[2], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_w));
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
}//FOC 計算 END

void motor_pwm_pulse(MotorParameter *motor)
{
    if((motor->gpio_angle_acc + motor->pwm_it_angle) < 60) // 累計移動角度
    {
        motor->gpio_angle_acc += motor->pwm_it_angle;
        motor->gpio_angle_acc = CLAMP(motor->gpio_angle_acc , 60, 0);
    }

    renew_adc(motor->const_h.adc_u_id, motor->adc_u);
    renew_adc(motor->const_h.adc_v_id, motor->adc_v);
    renew_adc(motor->const_h.adc_w_id, motor->adc_w);

    // 停轉判斷
    // 現在與上一個霍爾的總和與之前的總和相同，視為馬達靜止不動
    uint8_t hall_current = motor->gpio_hall_current;
    uint16_t hall_total = motor->pwm_hall_last*10 + hall_current;
    if(hall_total == motor->pwm_hall_acc)
    {
        motor->pwm_hall_same_acc++;
        if (motor->pwm_hall_same_acc >= 3000)
        {
            motor->pwm_hall_same_acc = 0;
            // timerclockvalue_onecycle_electric = 0;   // 歸零一電氣週期之時間
            motor->pwm_it_angle_acc = 59;               // 歸零角度累加
            motor->pi_speed.i1 = 0;                     // 重置i控制舊值
            motor->pi_speed.Fbk = 0;                    // 歸零速度實際值
            motor->pi_Iq.Out=0;
        }
    }
    else
    {
        motor->pwm_hall_same_acc = 0;
    }
    motor->pwm_hall_acc = hall_total;
    motor->pwm_hall_last = hall_current;

    // 外部VR轉速設定
    // Speed.Ref= 57 + (glo_ADC0_value[3]/30)*23;
    motor->pwm_count++;
    // 計算 速度PI (每100個PWM中斷)
    if(motor->pwm_count > 100)
    {
        // if(Speed.Fbk>0 && stop_flag==0)
        PI_run(&motor->pi_speed);
        motor->pi_speed_cmd = CLAMP((motor->pi_speed_cmd + motor->pi_speed.Out), 0.2, 0.15);
        // else if(Speed.Fbk==0 | stop_flag==1)
        //     motor->pi_speed_cmd=0.18;
        motor->pwm_count = 0;
    }

    Vector_control_calculate(motor);
}
