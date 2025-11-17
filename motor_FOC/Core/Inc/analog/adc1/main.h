#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "analog/adc1/basic.h"

extern ADC_HandleTypeDef hadc1;

void adc_set_zero_point(CURRENT_ADC *adc);
Result adc_renew(CURRENT_ADC *adc);
