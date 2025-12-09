#include "motor/main.h"
#include "tim.h"
#include "motor/ctrl_deg.h"
#include "motor/ctrl_foc.h"
#include "analog/adc1/main.h"
#include "motor/trigonometric.h"
#include "connectivity/fdcan/pkt_write.h"

static void hall_update(MotorParameter *motor)
{
    motor->hall_current =
          ((motor->const_h.Hall_GPIOx[0]->IDR & motor->const_h.Hall_GPIO_Pin_x[0]) ? 4U : 0U)
        | ((motor->const_h.Hall_GPIOx[1]->IDR & motor->const_h.Hall_GPIO_Pin_x[1]) ? 2U : 0U)
        | ((motor->const_h.Hall_GPIOx[2]->IDR & motor->const_h.Hall_GPIO_Pin_x[2]) ? 1U : 0U);
    if (motor->hall_current == 0 || motor->hall_current == 7)
    {
        motor->hall_current = UINT8_MAX;
        return;
    }
    vec_ctrl_hall_angle_trf(motor);
}

static void hall_check(MotorParameter *motor)
{
    // ccw clw check
    if (motor->hall_current == hall_seq_ccw[motor->hall_chk_last])
    {
        motor->rpm_feedback.reverse = 0;
        motor->hall_wrong = 0;
    }
    else if (motor->hall_current == hall_seq_clw[motor->hall_chk_last])
    {
        motor->rpm_feedback.reverse = 1;
        motor->hall_wrong = 0;
    }
    else
    {
        motor->hall_wrong++;
        if (motor->hall_wrong >= 6)
        {
            motor->hall_wrong = 6;
            motor->rpm_feedback.reverse = 0;
            motor->rpm_feedback.value = 0.0f;
            motor->foc_angle_itpl = 0.0f;
        }
    }
    motor->hall_chk_last = motor->hall_current;
}

static void rpm_update(MotorParameter *motor)
{
    motor->exti_hall_acc++;
    if (motor->exti_hall_acc >= MOTOR_RPM_CNT)
    {
        motor->exti_hall_acc = 0;
        uint32_t htim_cnt = __HAL_TIM_GET_COUNTER(motor->const_h.SPD_htimx);
        __HAL_TIM_SET_COUNTER(motor->const_h.SPD_htimx, 0);
        if (htim_cnt == 0)
        {
            motor->rpm_feedback.value = 0.0f;
            motor->foc_angle_itpl = 0.0f;
        }
        else
        {
            motor->rpm_feedback.value = motor->tfm_rpm_fbk / (float32_t)htim_cnt;
            motor->foc_angle_itpl = (!motor->rpm_feedback.reverse) ?
                 motor->tfm_foc_it_angle_itpl / (float32_t)htim_cnt :
                -motor->tfm_foc_it_angle_itpl / (float32_t)htim_cnt;
        }
    }
}

static void deg_update(MotorParameter *motor)
{
    switch (motor->mode_control)
    {
        case MOTOR_CTRL_120:
        {
            deg_ctrl_120_load(motor);
            break;
        }
        case MOTOR_CTRL_180:
        {
            deg_ctrl_180_load(motor);
            break;
        }
        default: break;
    }
}

void motor_hall_exti(MotorParameter *motor)
{
    hall_update(motor);
    hall_check(motor);
    rpm_update(motor);
    #ifndef MOTOR_FOC_SPIN_DEBUG
    deg_update(motor);
    #else
    if (expected && (motor->reverse == reverse))
    {
        motor->tim_angle_acc = 0.0f;
    }
    RESULT_CHECK_RET_VOID(vec_ctrl_hall_angle_trf(motor));
    deg_ctrl_load(motor);
    #endif
}

static void motor_adc_renew(MotorParameter *motor)
{
    RESULT_CHECK_RET_VOID(adc_renew(motor->adc_a));
    RESULT_CHECK_RET_VOID(adc_renew(motor->adc_b));
    RESULT_CHECK_RET_VOID(adc_renew(motor->adc_c));
}

static void ref_update(MotorParameter *motor)
{
    motor->mode_rotate = MOTOR_ROT_NORMAL;
    motor->rpm_set.reverse = motor->rpm_reference.reverse;
    motor->rpm_set.value = motor->rpm_reference.value;
    bool save_stop = (motor->rpm_feedback.value < motor->rpm_save_stop) ? 1 : 0;
    bool ref_fbk_same_dir = (motor->rpm_reference.reverse == motor->rpm_feedback.reverse) ? 1 : 0;
    switch (motor->dir_state)
    {
        case DIRECTION_NORMAL:
        {
            if (ref_fbk_same_dir) break;
            motor->dir_state = DIRECTION_SWITCHING;
        }
        case DIRECTION_SWITCHING:
        {
            if (save_stop)
            {
                motor->dir_state = DIRECTION_NORMAL;
                motor->mode_rotate = MOTOR_ROT_NORMAL;
                // PI_reset(&motor->pi_speed);
                // motor->pwm_duty_deg = 1.0f;
                break;
            }
            motor->mode_rotate = MOTOR_ROT_BREAK;
            // motor->rpm_set.reverse = motor->rpm_feedback.reverse;
            // motor->rpm_set.value = 0;
            break;
        }
    }
    // if (HAL_GetTick() - motor->alive_tick >= 1000)
    // {
    //     motor_set_rotate_mode(motor, MOTOR_ROT_COAST);
    // }
    motor->pi_speed.feedback = motor->rpm_feedback.value;
    switch (motor->mode_rotate)
    {
        case MOTOR_ROT_COAST:
        case MOTOR_ROT_BREAK:
        case MOTOR_ROT_LOCK_CHK:
        {
            PI_reset(&motor->pi_speed);
            break;
        }
        case MOTOR_ROT_NORMAL:
        {
            motor->pi_speed.reference = motor->rpm_set.value;
            PI_run(&motor->pi_speed);
            motor->pwm_duty_deg += motor->pi_speed.out;
            VAR_CLAMPF(motor->pwm_duty_deg, 0.0f, 1.0f);

            motor->pi_Iq.reference += motor->pi_speed.out * motor->tfm_duty_Iq;
            VAR_CLAMPF(motor->pi_Iq.reference, motor->pi_Iq.min, motor->pi_Iq.max);
            break;
        }
        case MOTOR_ROT_LOCK:
        {
            motor->mode_rotate = MOTOR_ROT_BREAK;
            if (save_stop) motor->mode_rotate = MOTOR_ROT_LOCK_CHK;
            break;
        }
    }

    motor->pi_Iq.reference = (!motor->rpm_reference.reverse) ?
        motor->const_h.rated_current : -motor->const_h.rated_current;
    // motor->tfm_duty_Iq = var_clampf((motor->tfm_duty_Iq + motor->pi_speed.out), 0.15f, 0.2f);
}

void motor_pwm_pulse(MotorParameter *motor)
{
    RESULT_CHECK_RET_VOID(vec_ctrl_hall_angle_chk(motor));
    motor_adc_renew(&motor_h);
    motor->tim_it_acc++;
    if (motor->tim_it_acc % 1000 == 0) ref_update(motor);
    if (
        motor->rpm_feedback.value == 0.0f &&
        motor->rpm_reference.value != 0.0f &&
        motor->tim_it_acc % 2000 == 0
    ) {
        if (motor->hall_current != UINT8_MAX)
        {
            if (!motor->rpm_reference.reverse)
            {
                motor->hall_start = hall_seq_ccw[motor->hall_start];
            }
            else
            {
                motor->hall_start = hall_seq_clw[motor->hall_start];
            }
            motor->hall_current = motor->hall_start;
            deg_update(motor);
        }
    }
    if (
        motor->fdcan_send &&
        motor->tim_it_acc % 2000 == 0
    ) fdcan_motor_send(motor);
    if (motor->tim_it_acc >= 20000) motor->tim_it_acc = 0;
    
    vec_ctrl_clarke(motor);
    vec_ctrl_park(motor);
    vec_ctrl_pi_id_iq(motor);
    vec_ctrl_ipark(motor);
    vec_ctrl_svgen(motor);
    vec_ctrl_svpwm(motor);
}

void motor_stop_trigger(MotorParameter *motor)
{
    motor->stop_spin_time = HAL_GetTick();
    motor->exti_hall_acc = 0;
    motor->rpm_feedback.value = 0;
    motor->pi_Iq.out = 0;
    motor->foc_angle_acc = 0.0f;
    PI_reset(&motor->pi_speed);
    motor->pwm_duty_deg = 0.0f;
}

static void init_setup(MotorParameter *motor)
{
    motor_init(motor);
    const float32_t PWM_tim_f =
        (float32_t)*motor->const_h.PWM_tim_clk /
        (float32_t)(motor->const_h.PWM_htimx->Init.Prescaler + 1U);

    const float32_t FOC_tim_f =
        (float32_t)*motor->const_h.IT20k_tim_clk /
        (float32_t)(motor->const_h.IT20k_htimx->Init.Prescaler + 1U);
    // ELE_tim_f：霍爾計時器的實際計數頻率 (Hz)
    // = ELE_timer_clock / (PSC + 1)
    const float32_t ELE_tim_f =
        (float32_t)*motor->const_h.SPD_tim_clk /
        (float32_t)(motor->const_h.SPD_htimx->Init.Prescaler + 1U);
    // FOC_tim_t：PWM 控制定時器每個計數週期的時間 (秒/計數)
    // = (PSC + 1) / TIM_timer_clock
    const float32_t FOC_tim_t =
        (float32_t)(motor->const_h.IT20k_htimx->Init.Prescaler + 1U) /
        (float32_t)*motor->const_h.IT20k_tim_clk;
    // ELE_tim_t：霍爾計時器每個計數週期的時間 (秒/計數)
    // = (PSC + 1) / ELE_timer_clock
    const float32_t ELE_tim_t =
        (float32_t)(motor->const_h.SPD_htimx->Init.Prescaler + 1U) /
        (float32_t)*motor->const_h.SPD_tim_clk;

    motor->dbg_pwm_freq = PWM_tim_f / (motor->const_h.PWM_htimx->Init.Period * 2);
    motor->dbg_tim_it_freq = FOC_tim_f / motor->const_h.IT20k_htimx->Init.Period;

    motor->tfm_pwm_period = motor->const_h.PWM_htimx->Init.Period;
    motor->tfm_rpm_fbk =
        ((float32_t)MOTOR_RPM_CNT / 6.0f * ELE_tim_f * 60.0f) / (float32_t)MOTOR_42BLF01_POLE;
    motor->tfm_foc_it_angle_itpl =
        FOC_tim_t / ELE_tim_t * (float32_t)(motor->const_h.IT20k_htimx->Init.Period) * PI_DIV_3;

    ERROR_CHECK_HAL_HANDLE(HAL_ADCEx_Calibration_Start(motor->adc_a->const_h.hadcx, ADC_SINGLE_ENDED));
    ERROR_CHECK_HAL_HANDLE(HAL_ADCEx_Calibration_Start(motor->adc_b->const_h.hadcx, ADC_SINGLE_ENDED));
    ERROR_CHECK_HAL_HANDLE(HAL_ADCEx_Calibration_Start(motor->adc_c->const_h.hadcx, ADC_SINGLE_ENDED));
    ERROR_CHECK_HAL_HANDLE(HAL_ADCEx_InjectedStart_IT(motor->adc_a->const_h.hadcx));

    ERROR_CHECK_HAL_HANDLE(HAL_TIM_Base_Start_IT(motor->const_h.IT20k_htimx));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_MID_TIM_CH_x,
        motor->const_h.PWM_htimx->Init.Period - 1);
    ERROR_CHECK_HAL_HANDLE(HAL_TIM_Base_Start(motor->const_h.PWM_htimx));
    ERROR_CHECK_HAL_HANDLE(HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_MID_TIM_CH_x));
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
        HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
    }
    HAL_TIM_Base_Start_IT(motor->const_h.SPD_htimx);
    // osDelay(1000);

    // adc_set_zero_point(motor_h.adc_a);
    // adc_set_zero_point(motor_h.adc_b);
    // adc_set_zero_point(motor_h.adc_c);
}

void StartMotorTask(void *argument)
{
    MotorParameter *motor = &motor_h;
    init_setup(motor);
    motor_set_rotate_mode(motor, MOTOR_ROT_NORMAL);
    motor_switch_ctrl(motor, MOTOR_CTRL_180);
    motor_set_speed(motor, 0, 500.0f);

    // motor_switch_ctrl(&motor_h, MOTOR_CTRL_FOC_RATED);
    // motor_hall_exti(&motor_h);
    StopTask();
}
