#include "analog/adc1/basic.h"

CURRENT_ADC adc_test = {
    .const_h = {
        .id = 3,
        .sensitive = 0.1f,
    },
};

CURRENT_ADC adc_0 = {
    .const_h = {
        .id = 0,
        .sensitive = 0.097f,
    },
    .current_trs = 1.0f,
};

CURRENT_ADC adc_1 = {
    .const_h = {
        .id = 1,
        .sensitive = 0.1f,
    },
    .current_trs = 1.0f,
};

CURRENT_ADC adc_2 = {
    .const_h = {
        .id = 2,
        .sensitive = 0.1f,
    },
    .current_trs = 1.0f,
};
