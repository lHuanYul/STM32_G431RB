#pragma once

#include "motor/basic.h"
#include "main/fn_state.h"

Result motor_foc_tim_setup(const MotorParameter *motor);
Result motor_foc_hall_update(MotorParameter *motor);
Result motor_foc_pwm_pulse(MotorParameter *motor);
