#include "motor/ctrl_foc.h"
#include "tim.h"
#include "motor/trigonometric.h"
#include "analog/adc1/main.h"

uint8_t sector_t[6] = {0};

Result motor_foc_tim_setup(const MotorParameter *motor)
{
    HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
    HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
    HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
    HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
    HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
    HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
    return RESULT_OK(NULL);
}

static inline Result motor_hall_to_angle(uint8_t hall, volatile float32_t *angle)
{
    switch(hall)
    {
        case 5:
        {
            *angle = 0.0f;
            break;
        }
        case 4:
        {
            *angle = 1.0f * PI_DIV_3;
            break;
        }
        case 6:
        {
            *angle = 2.0f * PI_DIV_3;
            break;
        }
        case 2:
        {
            *angle = 3.0f * PI_DIV_3;
            break;
        }
        case 3:
        {
            *angle = 4.0f * PI_DIV_3;
            break;
        }
        case 1:
        {
            *angle = 5.0f * PI_DIV_3;
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
    return RESULT_OK(NULL);
}

inline Result motor_foc_hall_update(MotorParameter *motor)
{
    RESULT_CHECK_RET_RES(motor_hall_to_angle(motor->exti_hall_curt, &motor->exti_hall_angal));
    if (motor->rot_drct_fbk == motor->rot_drct)
    {
        // rotated
        motor->foc_angle_acc = 0.0f;
    }
    sector_t[motor->exti_hall_curt-1] = motor->svgendq.Sector;
    return RESULT_OK(NULL);
}

inline void motor_foc_rot_stop(MotorParameter *motor)
{
    motor->pi_Iq.Out=0;
    motor->foc_angle_acc = 0.0f;
}

float32_t current_zero;
Result vec_ctrl_clarke(MotorParameter *motor)
{
    // clarke ideal
    // I alpha = Ia
    // I bata = (Ia+2Ib)/根號3
    // clarke nonideal
    // I alpha = 2/3Ia - 1/3Ib - 1/3Ic
    // I bata = (根號3/3)Ib - (根號3/3)Ic
    // (根號3/3) = 0.57735

    // 電流進 motor 為 正
    current_zero = (motor->adc_u.current + motor->adc_v.current + motor->adc_w.current) / 3.0f;
    motor->clarke.As = (motor->adc_u.current - current_zero);
    motor->clarke.Bs = (motor->adc_v.current - current_zero);
    motor->clarke.Cs = (motor->adc_w.current - current_zero);

    CLARKE_run_ideal(&motor->clarke);
    return RESULT_OK(NULL);
}

Result vec_ctrl_park(MotorParameter *motor)
{
    // park
    // Id = I alpha cos(theta) + I bata sin(theta)
    motor->foc_angle_acc += motor->foc_angle_itpl;
    motor->park.Alpha = motor->clarke.Alpha;
    motor->park.Beta = motor->clarke.Beta;
    RESULT_CHECK_RET_RES(trigo_sin_cosf(
        motor->exti_hall_angal + motor->foc_angle_acc + PI_DIV_2, // Here
        &motor->park.Sine, &motor->park.Cosine
    ));
    PARK_run(&motor->park);
    return RESULT_OK(NULL);
}

#define IQ_REF_ADD 0.0f
void vec_ctrl_pi_id_iq(MotorParameter *motor)
{
    if(motor->rpm_fbk > 0)
    {
        motor->pi_Id.Ref = 0;
        motor->pi_Iq.Ref = 0.1f;

        motor->pi_Id.Fbk = motor->park.Ds;
        PI_run(&motor->pi_Id); 
        motor->pi_Id.Out = clampf(motor->pi_Id.Out, -0.01f, 0.01f);
        
        // Speed.delta = Speed.Ref - Speed.Fbk;
        // Iq.Ref = 0.6 + (Speed.delta * Speed.Kp);//motor->pi_speed_cmd * Current_base

        // motor->pi_Iq.Ref = motor->pi_speed_cmd + IQ_REF_ADD;
        // motor->pi_Iq.Fbk = motor->park.Qs;
        // motor->pi_Iq.delta = (motor->pi_Iq.Ref - motor->pi_Iq.Fbk) * (motor->pi_Iq.Kp);
        // motor->pi_Iq.delta = CLAMP((motor->pi_Iq.delta), 0.1, -0.1);
        // motor->pi_Iq.Out = CLAMP((motor->pi_Iq.Ref + motor->pi_Iq.delta), 0.75, 0);
        // motor->pi_Iq.Ref = motor->pi_speed_cmd + IQ_REF_ADD;  // 外環給轉矩命令

        motor->pi_Iq.Fbk = motor->park.Qs;                    // q 軸量測
        PI_run(&motor->pi_Iq);                                // 統一用 PI_run + anti-windup
        motor->pi_Iq.Out = clampf(motor->pi_Iq.Out, 0.0f, 0.75f);
    }
    else
    {
        motor->pi_Id.Out = 0.0;
        motor->pi_Iq.Out = 0.2;
    }
}

Result vec_ctrl_ipark(MotorParameter *motor)
{
    // ipark
    // V alpha = Vd cos(theta) - Vq sin(theta)
    // V bata = Vd sin(theta) + Vq cos(theta)
    // ?
    motor->ipark.Vdref = clampf(motor->ipark.Vdref + motor->pi_Id.Out, -0.06f, 0.06f);
    motor->ipark.Vqref = motor->pi_Iq.Out;
    motor->ipark.Sine = motor->park.Sine;
    motor->ipark.Cosine = motor->park.Cosine;
    IPARK_run(&motor->ipark);
    RESULT_CHECK_RET_RES(trigo_atan(motor->ipark.Alpha, motor->ipark.Beta, &motor->elec_theta_rad));
    motor->elec_theta_rad = wrap_pi_pos(motor->elec_theta_rad, PI_MUL_2);
    // motor->elec_theta_deg = motor->elec_theta_rad * RAD_TO_DEG;
    return RESULT_OK(NULL);
}

void vec_ctrl_svgen(MotorParameter *motor)
{
    motor->svgendq.Ualpha = motor->ipark.Alpha;
    motor->svgendq.Ubeta = motor->ipark.Beta;
    SVGEN_run(&motor->svgendq);
}

Result vec_ctrl_vref(MotorParameter *motor)
{
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
    arm_status status = arm_sqrt_f32(
        motor->svgendq.Ualpha * motor->svgendq.Ualpha 
            + motor->svgendq.Ubeta * motor->svgendq.Ubeta,
        &motor->svpwm_Vref
    );
    if (status != ARM_MATH_SUCCESS) return RESULT_ERROR(RES_ERR_FAIL);

    // motor->svpwm_Vref = Iq.Out;
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

float32_t thete_t[6];
Result vec_ctrl_svpwm(MotorParameter *motor)
{
    float32_t theta = wrap_pi_pos(motor->elec_theta_rad, PI_DIV_3);
    // ? CHECK
    float32_t T1, T2;
    switch (motor->rot_drct)
    {
        case MOTOR_ROT_CLW:
        {
            RESULT_CHECK_RET_RES(trigo_sin_cosf(PI_DIV_3 - theta, &T1, NULL));
            RESULT_CHECK_RET_RES(trigo_sin_cosf(theta, &T2, NULL));
            break;
        }
        case MOTOR_ROT_CCW:
        {
            RESULT_CHECK_RET_RES(trigo_sin_cosf(theta, &T1, NULL));
            RESULT_CHECK_RET_RES(trigo_sin_cosf(PI_DIV_3 - theta, &T2, NULL));
            break;
        }
        default:
        {
            break;
        }
    }
    float32_t T0div2 = (1 - (T1 + T2)) / 2;
    switch(motor->svgendq.Sector)
    {
        case 2: // 0~59 4 cba
        {
            motor->pwm_duty_u = T0div2;
            motor->pwm_duty_v = T0div2 + T1;
            motor->pwm_duty_w = T0div2 + T1 + T2;
            break;
        }
        case 3: // 60~119 6 cba
        {
            motor->pwm_duty_u = T0div2 + T2;
            motor->pwm_duty_v = T0div2;
            motor->pwm_duty_w = T0div2 + T1 + T2;
            break;
        }
        case 1: // 120~179 2 acb
        {
            motor->pwm_duty_u = T0div2 + T1 + T2;
            motor->pwm_duty_v = T0div2;
            motor->pwm_duty_w = T0div2 + T1;
            break;
        }
        case 5: // 180~239 3 abc
        {
            motor->pwm_duty_u = T0div2 + T1 + T2;
            motor->pwm_duty_v = T0div2 + T2;
            motor->pwm_duty_w = T0div2;
            break;
        }
        case 4: // 240~299 1    bac
        {
            motor->pwm_duty_u = T0div2 + T1;
            motor->pwm_duty_v = T0div2 + T1 + T2;
            motor->pwm_duty_w = T0div2;
            break;
        }
        case 6: // 300~359 5  bca
        {
            motor->pwm_duty_u = T0div2;
            motor->pwm_duty_v = T0div2 + T1 + T2;
            motor->pwm_duty_w = T0div2 + T2;
            break;
        }
    }
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_u));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_v));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_w));
    return RESULT_OK(NULL);
}

uint32_t cycle[16] = {0};
#define CYCLE_CNT(id) ({cycle[id] = __HAL_TIM_GET_COUNTER(&htim2) - cycle[id-1];})

Result motor_foc_pwm_pulse(MotorParameter *motor)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    if (motor->dbg_pwm_count % 1 == 0)
    {
        CYCLE_CNT(4);
        // Thread - pwmIt - 2
        RESULT_CHECK_RET_RES(vec_ctrl_clarke(motor));
        CYCLE_CNT(5);
        // Thread - pwmIt - 3
        RESULT_CHECK_RET_RES(vec_ctrl_park(motor));
        CYCLE_CNT(6);
        // Thread - pwmIt - 4
        vec_ctrl_pi_id_iq(motor);
        CYCLE_CNT(7);
        // Thread - pwmIt - 5
        RESULT_CHECK_RET_RES(vec_ctrl_ipark(motor)); // !
        CYCLE_CNT(8);
        // Thread - pwmIt - 6
        vec_ctrl_svgen(motor);
        CYCLE_CNT(9);
        // Thread - pwmIt - 7
        RESULT_CHECK_RET_RES(vec_ctrl_vref(motor));  // !
        CYCLE_CNT(10);
        // Thread - pwmIt - 8
        RESULT_CHECK_RET_RES(vec_ctrl_svpwm(motor));
        CYCLE_CNT(11);
    }
    return RESULT_OK(NULL);
}
