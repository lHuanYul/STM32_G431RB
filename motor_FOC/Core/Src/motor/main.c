#include "motor/main.h"
#include "tim.h"
#include "motor/ctrl_120.h"
#include "motor/ctrl_foc.h"
#include "analog/adc1/main.h"
#include "motor/trigonometric.h"

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

void motor_hall_exti(MotorParameter *motor)
{
    motor->exti_hall_cnt++;
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
    float32_t htim_cnt = (float32_t)__HAL_TIM_GET_COUNTER(motor->const_h.SPD_htimx);
    __HAL_TIM_SET_COUNTER(motor->const_h.SPD_htimx, 0);
    if (htim_cnt == 0.0f) htim_cnt = 1.0f;
    motor->rpm_fbk = motor->tfm_rpm_fbk / htim_cnt;
    switch (motor->mode)
    {
        case MOTOR_CTRL_120:
        {
            motor_120_hall_update(motor);
            break;
        }
        case MOTOR_CTRL_FOC:
        {
            RESULT_CHECK_RET_VOID(motor_hall_to_angle(hall_current, &motor->exti_hall_angal));
            motor->foc_angle_itpl = motor->tfm_foc_it_angle_itpl / htim_cnt;
            motor_foc_hall_update(motor);
            break;
        }
    }
}

void motor_pwm_pulse(MotorParameter *motor)
{
    switch (motor->mode)
    {
        case MOTOR_CTRL_120:
        {
            break;
        }
        case MOTOR_CTRL_FOC:
        {
            
            break;
        }
    }
}

static void motor_init(MotorParameter *motor)
{
    const float32_t FOC_tim_f =
        (float32_t)*motor->const_h.FOC_tim_clk /
        (float32_t)(motor->const_h.FOC_htimx->Init.Prescaler + 1U);
    // ELE_tim_f：霍爾計時器的實際計數頻率 (Hz)
    // = ELE_timer_clock / (PSC + 1)
    const float32_t ELE_tim_f =
        (float32_t)*motor->const_h.SPD_tim_clk /
        (float32_t)(motor->const_h.SPD_htimx->Init.Prescaler + 1U);
    // TIM_tim_t：PWM 控制定時器每個計數週期的時間 (秒/計數)
    // = (PSC + 1) / TIM_timer_clock
    const float32_t FOC_tim_t =
        (float32_t)(motor->const_h.FOC_htimx->Init.Prescaler + 1U) /
        (float32_t)*motor->const_h.FOC_tim_clk;
    // ELE_tim_t：霍爾計時器每個計數週期的時間 (秒/計數)
    // = (PSC + 1) / ELE_timer_clock
    const float32_t ELE_tim_t =
        (float32_t)(motor->const_h.SPD_htimx->Init.Prescaler + 1U) /
        (float32_t)*motor->const_h.SPD_tim_clk;
    motor->dbg_foc_it_freq = FOC_tim_f / motor->const_h.FOC_htimx->Init.Period;
    motor->tfm_rpm_fbk =
        ELE_tim_f / (6.0f * (float32_t)MOTOR_42BLF01_POLE / 2.0f * MOTOR_42BLF01_GEAR) * 60.0f;
    motor->tfm_foc_it_angle_itpl =
        FOC_tim_t / ELE_tim_t * (float32_t)(motor->const_h.FOC_htimx->Init.Period) * PI_DIV_3;
    motor->pwm_duty_120 = 1.0f;

    HAL_TIM_Base_Start_IT(motor->const_h.PWM_htimx);
    HAL_TIM_Base_Start(motor->const_h.SPD_htimx);
    HAL_TIM_Base_Start(&htim2);
}

void motor_switch_ctrl(MotorParameter *motor, MotorCtrlMode ctrl)
{
    switch (ctrl)
    {
        case MOTOR_CTRL_120:
        {
            break;
        }
        case MOTOR_CTRL_FOC:
        {
            motor_foc_tim_setup(motor);
            break;
        }
        default:
        {
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
            break;
        }
    }
    motor->mode = ctrl;
}

void StartMotorTask(void *argument)
{
    while(HAL_GetTick() < 3000)
    {
        RESULT_CHECK_HANDLE(adc_renew(&motor_h.adc_u));
        RESULT_CHECK_HANDLE(adc_renew(&motor_h.adc_v));
        RESULT_CHECK_HANDLE(adc_renew(&motor_h.adc_w));
        osDelay(1);
    }
    adc_init(&motor_h.adc_u);
    adc_init(&motor_h.adc_v);
    adc_init(&motor_h.adc_w);
    motor_init(&motor_h);
    motor_h.pi_speed.Ref = 100.0f;
    // motor_h.reverse = true;
    motor_switch_ctrl(&motor_h, MOTOR_CTRL_120);
    motor_hall_exti(&motor_h);
    osDelay(3000);
    // motor_120_ctrl_stop(&motor_h);

    motor_switch_ctrl(&motor_h, MOTOR_CTRL_FOC);
    motor_hall_exti(&motor_h);

    StopTask();
}
