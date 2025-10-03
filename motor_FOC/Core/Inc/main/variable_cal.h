#pragma once

#include "main/config.h"
#include "main/fn_state.h"

#define PI          3.14159265358979323846f // 180 deg
#define MUL_2_PI    (2.0f * PI)     // 360 deg
#define DIV_PI_2    (PI / 2.0f)     // 90 deg
#define DIV_PI_3    (PI / 3.0f)     // 60 deg
#define DEG_TO_RAD  0.01745329252f  // π/180
#define RAD_TO_DEG  57.2957795131f  // 180/π
#define ONEbySQRT3  0.57735026918963f   // 1/sqrt(3)
#define DIV_1_3     (1.0f / 3.0f)
#define DIV_2_3     (2.0f / 3.0f)

uint16_t var_swap_u16(uint16_t value);
uint32_t var_swap_u32(uint32_t value);
