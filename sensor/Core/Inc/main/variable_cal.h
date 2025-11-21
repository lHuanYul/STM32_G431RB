#pragma once

#include "main/config.h"
#include "main/fn_state.h"

uint32_t var_swap_u32(uint32_t value);
void var_u32_to_u8_be(uint32_t value, uint8_t* u8);
uint32_t var_u8_to_u32_be(const uint8_t *u8);
void var_f32_to_u8_be(float32_t value, uint8_t* u8);
float32_t var_u8_to_f32_be(const uint8_t *u8);
float32_t var_clampf(float32_t val, float32_t min, float32_t max);
float32_t var_wrap_pos(float32_t x, float32_t value);
float32_t var_wrap_pi(float32_t x, float32_t value);
float32_t var_fabsf(float32_t x);

uint16_t var_swap_u16(uint16_t value);
