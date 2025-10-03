#pragma once

#include "motor/basic.h"
#include "main/fn_state.h"

Result trigo_sin_cosf(float32_t angle, float32_t *sin, float32_t *cos);
Result trigo_atan(float32_t x, float32_t y, float32_t *theta);
