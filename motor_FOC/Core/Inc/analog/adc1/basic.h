#pragma once

#include "main/config.h"

typedef struct CURRENT_ADC_CONST
{
    uint8_t id;
    float32_t sensitive;
} CURRENT_ADC_CONST;

typedef struct CURRENT_ADC
{
    CURRENT_ADC_CONST const_h;
    // 透過 sensitive 等常數計算出的值 用以直接換算
    float32_t current_trs;
    // 電流為0時的 adc 值
    float32_t zero;
    // 當前的 adc 值
    float32_t adc_value;
    // 換算後的電流值
    float32_t current;
} CURRENT_ADC;

extern CURRENT_ADC adc_test;
