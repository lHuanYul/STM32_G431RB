#include "main/main.h"
#include "adc.h"

AdcCurrentParameter adc_current_h[ADC_COUNT] = {
    {
        .model = &ADC_MODEL,
        // ADC1 CH11 PB12 0.097
        .basic = {
            .hadcx = &hadc1,
            .rankx = ADC_INJECTED_RANK_1,
        },
    },
    {
        .model = &ADC_MODEL,
        // ADC2 CH12 PB2
        .basic = {
            .hadcx = &hadc2,
            .rankx = ADC_INJECTED_RANK_1,
        },
    },
    {
        .model = &ADC_MODEL,
        // ADC1 CH14 PB11
        .basic = {
            .hadcx = &hadc1,
            .rankx = ADC_INJECTED_RANK_2,
        },
    },
};
