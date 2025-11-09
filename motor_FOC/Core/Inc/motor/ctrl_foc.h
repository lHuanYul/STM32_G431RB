#pragma once

#include "motor/basic.h"
#include "main/fn_state.h"

Result motor_angle_trsf(MotorParameter *motor);
void motor_foc_rot_stop(MotorParameter *motor);
Result motor_foc_pwm_pulse(MotorParameter *motor);
