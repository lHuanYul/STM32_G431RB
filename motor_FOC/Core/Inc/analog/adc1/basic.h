#pragma once

#include "main/config.h"

typedef struct ADC_OWN_CONST
{
    uint8_t id;
    float32_t sensitive;
} ADC_OWN_CONST;

typedef struct ADC_OWN
{
    ADC_OWN_CONST const_h;
    float32_t current_trs;
    float32_t zero;
    float32_t value;
} ADC_OWN;

extern ADC_OWN adc_test;
extern ADC_OWN adc_u;
extern ADC_OWN adc_v;
extern ADC_OWN adc_w;
