#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "motor/basic.h"

Result pwm_setup(const MotorParameter *motor);
Result motor_pwm_pulse(MotorParameter *motor);
