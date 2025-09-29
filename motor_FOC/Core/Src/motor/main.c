#include "motor/main.h"
#include "motor/basic.h"
#include "motor/it_hall.h"
#include "motor/it_timer.h"
#include "analog/adc1/main.h"

void StartMotorTask(void *argument)
{
    motor_hall_update(&motor_0);
    pwm_setup(&motor_0);
    motor_0.pi_speed.Ref = 20.0f;
    motor_0.pwm_duty_u = 0.5f;
    StopTask();
}
