#pragma once

#include "motor/basic.h"
#include "main/fn_state.h"

Result motor_angle_trsf(MotorParameter *motor);

void vec_ctrl_clarke(MotorParameter *motor);
Result vec_ctrl_park(MotorParameter *motor);
void vec_ctrl_pi_id_iq(MotorParameter *motor);
Result vec_ctrl_ipark(MotorParameter *motor);
void vec_ctrl_svgen(MotorParameter *motor);
Result vec_ctrl_svpwm(MotorParameter *motor);
void vec_ctrl_pwm(MotorParameter *motor);
