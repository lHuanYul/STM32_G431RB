#include "motor/main.h"
#include "motor/ctrl_120.h"
#include "motor/ctrl_foc.h"
#include "analog/adc1/main.h"

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
            HAL_TIM_PWM_Stop(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[i]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.htimx, motor->const_h.TIM_CHANNEL_x[i]);
            HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
        }
        return;
    }
    switch (motor->mode)
    {
        case MOTOR_CTRL_120:
        {
            motor_120_hall_update(motor);
            break;
        }
        case MOTOR_CTRL_FOC:
        {
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
            motor_foc_pwm_pulse(motor);
            break;
        }
    }
}

void StartMotorTask(void *argument)
{
    motor_h.pi_speed.Ref = 80.0f;
    motor_h.pwm_duty_u = 1.0f;
    // motor_h.reverse = true;

    HAL_TIM_Base_Start_IT(motor_h.const_h.htimx);
    HAL_TIM_Base_Start(motor_h.const_h.htimx);
    motor_hall_exti(&motor_h);
    motor_h.mode = MOTOR_CTRL_120;
    osDelay(5000);

    motor_foc_tim_setup(&motor_h);
    motor_hall_exti(&motor_h);
    motor_h.mode = MOTOR_CTRL_FOC;
    StopTask();
}
