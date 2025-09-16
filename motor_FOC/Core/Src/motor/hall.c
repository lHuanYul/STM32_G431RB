#include "motor/hall.h"

static void pos_calculate(MotorParameter *motor)
{
    // timer累加時間計數
    uint32_t *tim_cnt = motor->const_h.ELE_htimx->Instance->CNT;
    float per_electric_cyc_100ns = (float)*tim_cnt;
    *tim_cnt = 0;
    
    // 電氣週期算轉速，分鐘[3G=50,000,000 (計數轉秒)*60(秒轉分鐘)] / 轉速
    motor->pi_speed.Fbk = (6000000.0f / (per_electric_cyc_100ns * (MOTOR_POLE / 2)));
    // 單次PWM中斷時的角度變化 900000 = (1/pwm_freq)*50M*360
    motor->pwm_it_angle = ((180000.0f) / per_electric_cyc_100ns);
    motor->pi_speed.Fbk /= 6 ;
    // calculate speed every hall instead of  6 times
    motor->pi_speed.Fbk /= 4.4 ;
    // agv gear ratio 4.4
    motor->pwm_it_angle /= 6 ;
}

void motor_hall_exti(MotorParameter *motor)
{
    motor->gpio_hall_last = motor->gpio_hall_current;
    motor->gpio_hall_current =
          ((motor->const_h.Hall_GPIOx[0]->IDR & motor->const_h.Hall_GPIO_Pin_x[0]) ? 4U : 0U)
        | ((motor->const_h.Hall_GPIOx[1]->IDR & motor->const_h.Hall_GPIO_Pin_x[1]) ? 2U : 0U)
        | ((motor->const_h.Hall_GPIOx[2]->IDR & motor->const_h.Hall_GPIO_Pin_x[2]) ? 1U : 0U);
    if (motor->gpio_hall_current == 0 || motor->gpio_hall_current == 7)
    {
        Error_Handler();
        return;
    }

    uint16_t expected = (!motor->reverse)
        ? hall_seq_clw[motor->gpio_hall_last]
        : hall_seq_ccw[motor->gpio_hall_last];
    if (motor->gpio_hall_last == 0) // ? CHECK
    {
        motor->gpio_hall_last = expected;
    }
    if (motor->gpio_hall_current == expected)
    {
        motor->gpio_angle_acc = 0;
    }
    pos_calculate(motor);
}
