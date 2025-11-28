#include "motor/main.h"
#include "tim.h"
#include "motor/ctrl_deg.h"
#include "motor/ctrl_foc.h"
#include "analog/adc1/main.h"
#include "motor/trigonometric.h"

void motor_hall_exti(MotorParameter *motor)
{
    if (motor->mode_ctrl == MOTOR_CTRL_INIT) return;
    // hall update
    uint8_t hall_last = motor->exti_hall_curt;
    uint8_t hall_current =
          ((motor->const_h.Hall_GPIOx[0]->IDR & motor->const_h.Hall_GPIO_Pin_x[0]) ? 4U : 0U)
        | ((motor->const_h.Hall_GPIOx[1]->IDR & motor->const_h.Hall_GPIO_Pin_x[1]) ? 2U : 0U)
        | ((motor->const_h.Hall_GPIOx[2]->IDR & motor->const_h.Hall_GPIO_Pin_x[2]) ? 1U : 0U);
    motor->exti_hall_last = hall_last;
    motor->exti_hall_curt = hall_current;
    if (hall_current == 0 || hall_current == 7) 
    {
        uint8_t i;
        for (i = 0; i < 3; i++)
        {
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
        }
        return;
    }
    // ccw clw check
    if (
           motor->exti_hall_curt == hall_seq_ccw[motor->exti_hall_last]
        || motor->exti_hall_last == 0
    ) {
        motor->rpm_fbk.reverse = 0;
        motor->hall_offline = 0;
    }
    else if (motor->exti_hall_curt == hall_seq_clw[motor->exti_hall_last])
    {
        motor->rpm_fbk.reverse = 1;
        motor->hall_offline = 0;
    }
    else
    {
        motor->hall_offline++;
        motor->rpm_fbk.reverse = 0;
        motor->rpm_fbk.value = 0.0f;
        motor->foc_angle_itpl = 0;
    }
    // if (motor->hall_offline) return;
    vec_ctrl_hall_angle_trf(motor);
    // rpm calculate
    motor->exti_hall_acc++;
    if (motor->exti_hall_acc >= 6)
    {
        motor->exti_hall_acc = 0;
        uint32_t htim_cnt = __HAL_TIM_GET_COUNTER(motor->const_h.SPD_htimx);
        __HAL_TIM_SET_COUNTER(motor->const_h.SPD_htimx, 0);
        if (htim_cnt == 0)
        {
            motor->rpm_fbk.reverse = 0;
            motor->rpm_fbk.value = 0.0f;
            motor->foc_angle_itpl = 0.0f;
        }
        else
        {
            motor->rpm_fbk.value = motor->tfm_rpm_fbk / (float32_t)htim_cnt;
            motor->foc_angle_itpl = (!motor->rpm_fbk.reverse) ?
                 motor->tfm_foc_it_angle_itpl / (float32_t)htim_cnt :
                -motor->tfm_foc_it_angle_itpl / (float32_t)htim_cnt;
        }
    }
    //
    #ifndef MOTOR_FOC_SPIN_DEBUG
    if (
           (motor->mode_ctrl == MOTOR_CTRL_LOCK)
        || (motor->mode_ctrl == MOTOR_CTRL_120)
        || (motor->mode_ctrl == MOTOR_CTRL_180)
    ) deg_ctrl_load(motor);
    #else
    if (expected && (motor->reverse == reverse))
    {
        motor->tim_angle_acc = 0.0f;
    }
    RESULT_CHECK_RET_VOID(vec_ctrl_hall_angle_trf(motor));
    deg_ctrl_load(motor);
    #endif
}

// static inline void stop_check(MotorParameter *motor)
// {
//     // 停轉判斷
//     // 現在與上一個霍爾的總和與之前的總和相同，視為馬達靜止不動
//     uint8_t current = motor->exti_hall_curt;
//     uint16_t total = motor->tim_hall_last * 10 + current;
//     if(total == motor->tim_hall_total)
//     {
//         motor->stop_spin_acc++;
//         if (motor->stop_spin_acc >= MOTOR_STOP_TRI)
//         {
//             motor->stop_spin_acc = 0;
//             motor->pi_spd.fbk = 0;    // 歸零速度實際值
//             __HAL_TIM_SET_COUNTER(motor->const_h.SPD_htimx, 0);
//             motor->pi_spd.Uil = 0;     // 重置i控制舊值
//             motor_foc_rot_stop(motor);
//         }
//     }
//     else motor->stop_spin_acc = 0;
//     motor->tim_hall_last = current;
//     motor->tim_hall_total = total;
// }

void motor_adc_renew(MotorParameter *motor)
{
    RESULT_CHECK_RET_VOID(adc_renew(motor->adc_a));
    RESULT_CHECK_RET_VOID(adc_renew(motor->adc_b));
    RESULT_CHECK_RET_VOID(adc_renew(motor->adc_c));
}

void motor_pwm_pulse(MotorParameter *motor)
{
    if (motor->mode_ctrl == MOTOR_CTRL_INIT) return;
    // motor_adc_renew(motor);
    RESULT_CHECK_RET_VOID(vec_ctrl_hall_angle_chk(motor));
    motor->tim_it_acc++;
    if (motor->tim_it_acc >= 100)
    {
        motor->tim_it_acc = 0;
        motor->pi_spd.ref = (motor->rpm_ref.reverse == motor->rpm_fbk.reverse) ?
            motor->rpm_ref.value : 0.0f;
        motor->pi_spd.fbk = motor->rpm_fbk.value;
        // stop_check(motor);
        PI_run(&motor->pi_spd);
        // motor->spd_Iq_set = var_clampf((motor->spd_Iq_set + motor->pi_spd.out), 0.15f, 0.2f);
        motor->spd_Iq_set = 1.9f/4000.0f;
    }
    vec_ctrl_clarke(motor);
    vec_ctrl_park(motor);
    vec_ctrl_pi_id_iq(motor);
    vec_ctrl_ipark(motor);
    vec_ctrl_svgen(motor);
    vec_ctrl_svpwm(motor);
    #ifndef MOTOR_FOC_SPIN_DEBUG
    if (motor->mode_ctrl == MOTOR_CTRL_FOC_RATED) vec_ctrl_load(motor);
    #endif
}

void motor_stop_trigger(MotorParameter *motor)
{
    motor->stop_spin_acc = 0;
    motor->rpm_fbk.reverse = 0;
    motor->rpm_fbk.value = 0;
    motor->pi_spd.Uil = 0;     // 重置i控制舊值

    motor->pi_Iq.out = 0;
    motor->foc_angle_acc = 0.0f;
}

static void motor_setup(MotorParameter *motor)
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
    // TIM_tim_t：PWM 控制定時器每個計數週期的時間 (秒/計數)
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
    // ELE_tim_f / (6.0f * (float32_t)MOTOR_42BLF01_POLE / 2.0f * MOTOR_42BLF01_GEAR) * 60.0f;
    motor->tfm_rpm_fbk =
        ELE_tim_f / (float32_t)MOTOR_42BLF01_POLE * 20.0f;
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
    ERROR_CHECK_HAL_HANDLE(HAL_TIM_Base_Start_IT(motor->const_h.SPD_htimx));

    osDelay(1000);
    adc_set_zero_point(motor_h.adc_a);
    adc_set_zero_point(motor_h.adc_b);
    adc_set_zero_point(motor_h.adc_c);
}

void StartMotorTask(void *argument)
{
    motor_setup(&motor_h);
    motor_set_speed(&motor_h, 250.0f);

    motor_switch_ctrl(&motor_h, MOTOR_CTRL_180);
    motor_hall_exti(&motor_h);
    osDelay(2000);

    // motor_switch_ctrl(&motor_h, MOTOR_CTRL_FOC_RATED);
    // motor_hall_exti(&motor_h);
    StopTask();
}
