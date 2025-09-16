#include "main/variable_cal.h"

// 32 bit --------------------------------------------------------------------------------

uint32_t var_comb_u32(const void* src)
{
    const uint8_t *out = (const uint8_t *)src;
    return    ((uint32_t)out[0] << 24)
            | ((uint32_t)out[1] << 16)
            | ((uint32_t)out[2] <<  8)
            | ((uint32_t)out[3] <<  0);
}

/**
 * @brief 交換 32-bit 整數的大小端
 *
 * @param value 要交換大小端的 32-bit 值
 * 
 * @return uint32_t 交換後的 32-bit 值
 */
uint32_t var_swap_u32(uint32_t value)
{
    return    ((value & 0x000000FFU) << 24)
            | ((value & 0x0000FF00U) <<  8)
            | ((value & 0x00FF0000U) >>  8)
            | ((value & 0xFF000000U) >> 24);
}

int32_t var_swap_i32(int32_t value)
{
    int32_t out;
    memcpy(&out, &value, sizeof out);
    out = var_swap_u32(out);
    memcpy(&value, &out, sizeof value);
    return value;
}

float var_swap_f32(float value)
{
    uint32_t out;
    memcpy(&out, &value, sizeof out);
    out = var_swap_u32(out);
    memcpy(&value, &out, sizeof value);
    return value;
}

// 16 bit --------------------------------------------------------------------------------

uint16_t var_comb_u16(const void* src)
{
    const uint8_t *out = (const uint8_t *)src;
    return    ((uint16_t)out[2] <<  8)
            | ((uint16_t)out[3] <<  0);
}

/**
 * @brief 交換 16-bit 整數的大小端
 *
 * @param value 要交換大小端的 16-bit 值
 * 
 * @return uint16_t 交換後的 16-bit 值
 */
uint16_t var_swap_u16(uint16_t value)
{
    return    ((value & 0x00FFU) << 8)
            | ((value & 0xFF00U) >> 8);
}



