#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "analog/adc1/basic.h"

extern ADC_HandleTypeDef hadc1;

void adc_set_zero_point(ADC_PARAMETER *adc);
Result adc_renew(ADC_PARAMETER *adc);
