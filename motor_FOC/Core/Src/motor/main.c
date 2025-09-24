#include "motor/main.h"
#include "motor/basic.h"
#include "motor/it_timer.h"
#include "analog/adc1/main.h"

void StartMotorTask(void *argument)
{
    pwm_setup(&motor_0);
    motor_0.pi_speed.Ref = 20.0f;
    StopTask();
}
