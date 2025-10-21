#include "analog/adc1/basic.h"

ADC_OWN adc_test = {
    .const_h = {
        .id = 3,
        .sensitive = 0.124f,
    },
};

ADC_OWN adc_u = {
    .const_h = {
        .id = 0,
        .sensitive = 0.124f,
    },
};

ADC_OWN adc_v = {
    .const_h = {
        .id = 1,
        .sensitive = 0.124f,
    },
};

ADC_OWN adc_w = {
    .const_h = {
        .id = 2,
        .sensitive = 0.124f,
    },
};
