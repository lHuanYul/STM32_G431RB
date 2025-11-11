#pragma once

#include "motor/basic.h"
#include "main/fn_state.h"

void motor_hall_exti(MotorParameter *motor);
void motor_pwm_pulse(MotorParameter *motor);
void motor_stop_trigger(MotorParameter *motor);

void motor_set_direction(MotorParameter *motor, bool reverse);
void motor_switch_ctrl(MotorParameter *motor, MotorCtrlMode ctrl);
