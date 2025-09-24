#pragma once

#include "main/config.h"
#include "main/fn_state.h"

#define PI          3.14159265358979323846f
#define MUL_2_PI    (2.0f * PI)
#define DIV_PI_2    (PI / 2.0f)
#define DIV_PI_3    (PI / 3.0f)
#define DEG_TO_RAD  0.01745329252f   // π/180
#define RAD_TO_DEG  57.2957795131f    // 180/π

float TableSearch_sin(float input);
float TableSearch_atan(float theta);
float TableSearch_atan2(float y, float x);
