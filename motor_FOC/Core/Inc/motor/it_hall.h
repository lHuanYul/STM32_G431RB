#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "motor/basic.h"

Result motor_hall_update(MotorParameter *motor);
Result motor_hall_exti(MotorParameter *motor);
