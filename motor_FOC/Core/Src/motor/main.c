#include "motor/main.h"
#include "motor/ctrl_120.h"
#include "motor/ctrl_foc.h"
#include "analog/adc1/main.h"

static Result tim_setup(const MotorParameter *motor)
{
    HAL_TIM_Base_Start_IT(motor->const_h.htimx);
    HAL_TIM_Base_Start(motor->const_h.ELE_htimx);
    motor_foc_tim_setup(motor);
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
    if (hall_current == 0 || hall_current == 7) return;
    // motor_120_hall_update(motor);
    motor_foc_hall_update(motor);
}

void motor_pwm_pulse(MotorParameter *motor)
{
    motor_foc_pwm_pulse(motor);
}

void StartMotorTask(void *argument)
{
    tim_setup(&motor_0);
    motor_hall_exti(&motor_0);
    motor_0.pi_speed.Ref = 20.0f;
    motor_0.pwm_duty_u = 1.0f;
    StopTask();
}
