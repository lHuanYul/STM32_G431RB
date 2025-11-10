#include "motor/ctrl_foc.h"
#include "tim.h"
#include "motor/trigonometric.h"
#include "analog/adc1/main.h"

inline Result motor_angle_trsf(MotorParameter *motor)
{
    switch(motor->exti_hall_curt)
    {
        case 4:
        {
            motor->exti_hall_angal = 0.0f;
            break;
        }
        case 6:
        {
            motor->exti_hall_angal = 1.0f * PI_DIV_3;
            break;
        }
        case 2:
        {
            motor->exti_hall_angal = 2.0f * PI_DIV_3;
            break;
        }
        case 3:
        {
            motor->exti_hall_angal = 3.0f * PI_DIV_3;
            break;
        }
        case 1:
        {
            motor->exti_hall_angal = 4.0f * PI_DIV_3;
            break;
        }
        case 5:
        {
            motor->exti_hall_angal = 5.0f * PI_DIV_3;
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
    return RESULT_OK(NULL);
}

inline void motor_foc_rot_stop(MotorParameter *motor)
{
    motor->pi_Iq.Out = 0;
    motor->tim_angle_acc = 0.0f;
}

float32_t current_zero;
void vec_ctrl_clarke(MotorParameter *motor)
{
    // 電流進motor為 正
    current_zero = (motor->adc_a->current + motor->adc_b->current + motor->adc_c->current) / 3.0f;
    motor->clarke.As = (motor->adc_a->current - current_zero);
    motor->clarke.Bs = (motor->adc_b->current - current_zero);
    motor->clarke.Cs = (motor->adc_c->current - current_zero);

    CLARKE_run_ideal(&motor->clarke);
    return;
}

Result vec_ctrl_park(MotorParameter *motor)
{
    motor->park.Alpha = motor->clarke.Alpha;
    motor->park.Beta = motor->clarke.Beta;
    motor->tim_angle_acc = clampf(motor->tim_angle_acc + motor->tim_angle_itpl, -PI_DIV_3, PI_DIV_3);
    // 電壓向量應提前90度 +PI_DIV_2
    RESULT_CHECK_HANDLE(trigo_sin_cosf(
        motor->exti_hall_angal + motor->tim_angle_acc + MOTOR_42BLF01_ANGLE + PI_DIV_2,
        &motor->park.Sin, &motor->park.Cos
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
        motor->pi_Iq.Ref = 0.2f;

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

        motor->pi_Iq.Fbk = motor->park.Qs;
        PI_run(&motor->pi_Iq);
        motor->pi_Iq.Out = clampf(motor->pi_Iq.Out, -0.75f, 0.75f);
    }
    else
    {
        motor->pi_Id.Out = 0.0f;
        motor->pi_Iq.Out = 0.2f;
    }
}

Result vec_ctrl_ipark(MotorParameter *motor)
{
    motor->ipark.Vdref = 0.0;
    motor->ipark.Vqref = 0.2;
    if (motor->reverse) motor->ipark.Vqref *= -1;
    // motor->ipark.Vdref = clampf(motor->ipark.Vdref + motor->pi_Id.Out, -0.06f, 0.06f);
    // motor->ipark.Vqref = motor->pi_Iq.Out;
    motor->ipark.Sin = motor->park.Sin;
    motor->ipark.Cos = motor->park.Cos;
    IPARK_run(&motor->ipark);
    // motor->ipark.Beta *= -1;
    RESULT_CHECK_RET_RES(trigo_atan(motor->ipark.Alpha, motor->ipark.Beta, &motor->elec_theta_rad));
    motor->elec_theta_rad = wrap_positive(motor->elec_theta_rad, PI_MUL_2);
    return RESULT_OK(NULL);
}

float32_t sec_chk[30] = {0};
uint8_t chk_cnt = 0;
uint8_t sec_mem = 0;
void vec_ctrl_svgen(MotorParameter *motor)
{
    motor->svgendq.Ualpha = motor->ipark.Alpha;
    motor->svgendq.Ubeta  = motor->ipark.Beta ;
    SVGEN_run(&motor->svgendq);

    if (motor->svgendq.Sector != sec_mem)
    {
        sec_chk[chk_cnt++] = motor->exti_hall_curt;
        // sec_chk[chk_cnt++] = motor->elec_theta_rad;
        sec_chk[chk_cnt++] = motor->svgendq.Sector;
        if (chk_cnt >= 30) chk_cnt = 0;
    }
    sec_mem = motor->svgendq.Sector;
}

Result vec_ctrl_svpwm(MotorParameter *motor)
{
    float32_t vref;
    if (
        arm_sqrt_f32(
            motor->svgendq.Ualpha * motor->svgendq.Ualpha + motor->svgendq.Ubeta * motor->svgendq.Ubeta,
            &vref
        ) != ARM_MATH_SUCCESS
    ) return RESULT_ERROR(RES_ERR_FAIL);
    float32_t theta = wrap_positive(motor->elec_theta_rad, PI_DIV_3);
    // T1: 第一個有源向量導通時間 在該sector內靠近前一個主向量的時間比例(由sin(π/3−θ)決定)
    // T2: 第二個有源向量導通時間 在該sector內靠近下一個主向量的時間比例(由sin(θ)決定)
    float32_t T1, T2;
    if (!motor->reverse)
    {
        RESULT_CHECK_RET_RES(trigo_sin_cosf(PI_DIV_3 - theta, &T1, NULL));
        RESULT_CHECK_RET_RES(trigo_sin_cosf(theta, &T2, NULL));
    }
    else
    {
        RESULT_CHECK_RET_RES(trigo_sin_cosf(theta, &T1, NULL));
        RESULT_CHECK_RET_RES(trigo_sin_cosf(PI_DIV_3 - theta, &T2, NULL));
    }
    vref = 1.0f;
    T1 *= vref;
    T2 *= vref;
    // T0div2: 零向量時間的一半 將整個零向量時間平均分配到PWM週期的前後兩端 讓波形中心對稱
    float32_t T0div2 = (1.0f - (T1 + T2)) * 0.5f;
    switch (motor->svgendq.Sector)
    {
        case 6:
        {
            motor->pwm_duty_u = T0div2 + T1 + T2;
            motor->pwm_duty_v = T0div2 + T2;
            motor->pwm_duty_w = T0div2;
            break;
        }
        case 2:
        {
            motor->pwm_duty_u = T0div2 + T1;
            motor->pwm_duty_v = T0div2 + T1 + T2;
            motor->pwm_duty_w = T0div2;
            break;
        }
        case 3:
        {
            motor->pwm_duty_u = T0div2;
            motor->pwm_duty_v = T0div2 + T1 + T2;
            motor->pwm_duty_w = T0div2 + T2;
            break;
        }
        case 1:
        {
            motor->pwm_duty_u = T0div2;
            motor->pwm_duty_v = T0div2 + T1;
            motor->pwm_duty_w = T0div2 + T1 + T2;
            break;
        }
        case 5:
        {
            motor->pwm_duty_u = T0div2 + T2;
            motor->pwm_duty_v = T0div2;
            motor->pwm_duty_w = T0div2 + T1 + T2;
            break;
        }
        case 4:
        {
            motor->pwm_duty_u = T0div2 + T1 + T2;
            motor->pwm_duty_v = T0div2;
            motor->pwm_duty_w = T0div2 + T1;
            break;
        }
    }
    motor->pwm_duty_u = clampf(motor->pwm_duty_u, 0.0f, 0.98f);
    motor->pwm_duty_v = clampf(motor->pwm_duty_v, 0.0f, 0.98f);
    motor->pwm_duty_w = clampf(motor->pwm_duty_w, 0.0f, 0.98f);
    #ifndef MOTOR_FOC_SPIN_DEBUG
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_u));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_v));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_w));
    #endif
    return RESULT_OK(NULL);
}

uint32_t cycle[16] = {0};
#define CYCLE_CNT(id) ({cycle[id] = __HAL_TIM_GET_COUNTER(&htim3) - cycle[id-1];})
Result motor_foc_pwm_pulse(MotorParameter *motor)
{
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    CYCLE_CNT(4);
    // Thread - pwmIt - 2
    vec_ctrl_clarke(motor);
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
    CYCLE_CNT(10);
    // Thread - pwmIt - 8
    RESULT_CHECK_RET_RES(vec_ctrl_svpwm(motor));
    CYCLE_CNT(11);
    return RESULT_OK(NULL);
}
