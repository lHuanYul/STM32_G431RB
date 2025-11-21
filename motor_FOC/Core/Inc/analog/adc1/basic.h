#pragma once

#include "main/config.h"

typedef struct ADC_PARAMETER_CONST
{
    ADC_HandleTypeDef *hadcx;
    uint32_t rankx;
    float32_t sensitive;
} ADC_PARAMETER_CONST;

typedef struct ADC_PARAMETER
{
    ADC_PARAMETER_CONST const_h;
    // 透過 sensitive 等常數計算出的值 用以直接換算
    float32_t current_trs;
    // 電流為0時的 adc 值
    uint16_t val_zero;
    // 當前的 adc 值
    uint16_t adc_value;
    // 換算後的電流值
    float32_t current;
} ADC_PARAMETER;

extern ADC_PARAMETER adc_test;
extern ADC_PARAMETER adc_0;
extern ADC_PARAMETER adc_1;
extern ADC_PARAMETER adc_2;
