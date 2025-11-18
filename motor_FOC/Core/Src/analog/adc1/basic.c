#include "analog/adc1/basic.h"
#include "adc.h"

CURRENT_ADC adc_test = {
    .const_h = {
        .hadcx = &hadc1,
        .rankx = 3,
        .sensitive = 0.1f,
    },
};

// CH11 PB12 0.097
CURRENT_ADC adc_0 = {
    .const_h = {
        .hadcx = &hadc1,
        .rankx = ADC_INJECTED_RANK_1,
        .sensitive = 0.1f,
    },
    .current_trs = 1.0f,
};

// CH12 PB1
CURRENT_ADC adc_1 = {
    .const_h = {
        .hadcx = &hadc1,
        .rankx = ADC_INJECTED_RANK_2,
        .sensitive = 0.1f,
    },
    .current_trs = 1.0f,
};

// CH14 PB11
CURRENT_ADC adc_2 = {
    .const_h = {
        .hadcx = &hadc1,
        .rankx = ADC_INJECTED_RANK_3,
        .sensitive = 0.1f,
    },
    .current_trs = 1.0f,
};
