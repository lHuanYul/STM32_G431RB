#pragma once

#include "main/config.h"

typedef struct CURRENT_ADC_CONST
{
    ADC_HandleTypeDef *hadcx;
    uint32_t rankx;
    float32_t sensitive;
} CURRENT_ADC_CONST;

typedef struct CURRENT_ADC
{
    CURRENT_ADC_CONST const_h;
    // 透過 sensitive 等常數計算出的值 用以直接換算
    float32_t current_trs;
    // 電流為0時的 adc 值
    uint16_t val_zero;
    // 當前的 adc 值
    uint16_t adc_value;
    // 換算後的電流值
    float32_t current;
} CURRENT_ADC;

extern CURRENT_ADC adc_test;
extern CURRENT_ADC adc_0;
extern CURRENT_ADC adc_1;
extern CURRENT_ADC adc_2;
