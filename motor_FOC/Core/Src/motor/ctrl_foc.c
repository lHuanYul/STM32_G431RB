#include "motor/ctrl_foc.h"
#include "tim.h"
#include "motor/trigonometric.h"
#include "analog/adc1/main.h"

static const float32_t hall_elec_angle[8] = {
    F32_MAX,
    4.0f * PI_DIV_3,
    2.0f * PI_DIV_3,
    3.0f * PI_DIV_3,
    0.0f,
    5.0f * PI_DIV_3,
    1.0f * PI_DIV_3,
    F32_MAX,
};

inline void vec_ctrl_hall_angle_trf(MotorParameter *motor)
{
    motor->exti_hall_rad = hall_elec_angle[motor->exti_hall_curt];
    // switch(motor->exti_hall_curt)
    // {
    //     case 4:
    //     {
    //         motor->exti_hall_rad = 0.0f;
    //         break;
    //     }
    //     case 6:
    //     {
    //         motor->exti_hall_rad = 1.0f * PI_DIV_3;
    //         break;
    //     }
    //     case 2:
    //     {
    //         motor->exti_hall_rad = 2.0f * PI_DIV_3;
    //         break;
    //     }
    //     case 3:
    //     {
    //         motor->exti_hall_rad = 3.0f * PI_DIV_3;
    //         break;
    //     }
    //     case 1:
    //     {
    //         motor->exti_hall_rad = 4.0f * PI_DIV_3;
    //         break;
    //     }
    //     case 5:
    //     {
    //         motor->exti_hall_rad = 5.0f * PI_DIV_3;
    //         break;
    //     }
    //     default: break;;
    // }
}

inline Result vec_ctrl_hall_angle_chk(MotorParameter *motor)
{
    if (motor->exti_hall_rad == F32_MAX) return RESULT_ERROR(RES_ERR_NOT_FOUND);
    return RESULT_OK(motor);
}

float32_t current_zero;
inline void vec_ctrl_clarke(MotorParameter *motor)
{
    // 電流進motor為 正
    current_zero = (motor->adc_a->current + motor->adc_b->current + motor->adc_c->current) / 3.0f;
    motor->clarke.As = (motor->adc_a->current - current_zero);
    motor->clarke.Bs = (motor->adc_b->current - current_zero);
    motor->clarke.Cs = (motor->adc_c->current - current_zero);
    CLARKE_run_ideal(&motor->clarke);
    return;
}

inline void vec_ctrl_park(MotorParameter *motor)
{
    motor->park.Alpha = motor->clarke.Alpha;
    motor->park.Beta = motor->clarke.Beta;
    motor->foc_angle_acc = var_clampf(motor->foc_angle_acc + motor->foc_angle_itpl, -PI_DIV_3, PI_DIV_3);
    // 電壓向量應提前90度 +PI_DIV_2
    RESULT_CHECK_HANDLE(trigo_sin_cosf(
        motor->exti_hall_rad + motor->foc_angle_acc + MOTOR_42BLF01_ANGLE + PI_DIV_2,
        &motor->park.Sin, &motor->park.Cos
    ));
    PARK_run(&motor->park);
}

inline void vec_ctrl_pi_id_iq(MotorParameter *motor)
{
    if(motor->pi_spd.Fbk != 0.0f)
    {
        motor->pi_Id.Fbk = motor->park.Ds;
        PI_run(&motor->pi_Id);

        motor->pi_Iq.Ref = motor->spd_Iq_set;
        motor->pi_Iq.Fbk = motor->park.Qs;
        PI_run(&motor->pi_Iq);
    }
    else
    {
        motor->pi_Iq.Out = motor->pi_spd.Ref < 0 ?
            -motor->const_h.peak_current : motor->const_h.peak_current;
    }
}

inline void vec_ctrl_ipark(MotorParameter *motor)
{
    // motor->ipark.Vdref = var_clampf(motor->ipark.Vdref + motor->pi_Id.Out, -0.06f, 0.06f);
    motor->ipark.Vdref = motor->pi_Id.Out;
    motor->ipark.Vqref = motor->pi_Iq.Out;
    motor->ipark.Sin = motor->park.Sin;
    motor->ipark.Cos = motor->park.Cos;
    IPARK_run(&motor->ipark);
    RESULT_CHECK_HANDLE(trigo_atan(motor->ipark.Alpha, motor->ipark.Beta, &motor->elec_theta_rad));
    motor->elec_theta_rad = var_wrap_pos(motor->elec_theta_rad, PI_MUL_2);
}

float32_t sec_chk[30] = {0};
uint8_t chk_cnt = 0;
uint8_t sec_mem = 0;
inline void vec_ctrl_svgen(MotorParameter *motor)
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

inline void vec_ctrl_svpwm(MotorParameter *motor)
{
    if (
        arm_sqrt_f32(
            motor->svgendq.Ualpha * motor->svgendq.Ualpha + motor->svgendq.Ubeta * motor->svgendq.Ubeta,
            &motor->v_ref
        ) != ARM_MATH_SUCCESS
    ) while (1) {};
    float32_t theta = var_wrap_pos(motor->elec_theta_rad, PI_DIV_3);
    // T1: 第一個有源向量導通時間 在該sector內靠近前一個主向量的時間比例(由sin(π/3−θ)決定)
    // T2: 第二個有源向量導通時間 在該sector內靠近下一個主向量的時間比例(由sin(θ)決定)
    float32_t T1, T2;
    RESULT_CHECK_HANDLE(trigo_sin_cosf(PI_DIV_3 - theta, &T1, NULL));
    RESULT_CHECK_HANDLE(trigo_sin_cosf(theta, &T2, NULL));
    T1 *= motor->v_ref;
    T2 *= motor->v_ref;
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
}

inline void vec_ctrl_load(MotorParameter *motor)
{
    motor->pwm_duty_u = var_clampf(motor->pwm_duty_u, 0.0f, 1.0f);
    motor->pwm_duty_v = var_clampf(motor->pwm_duty_v, 0.0f, 1.0f);
    motor->pwm_duty_w = var_clampf(motor->pwm_duty_w, 0.0f, 1.0f);
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_u));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_v));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2], (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_w));
}
