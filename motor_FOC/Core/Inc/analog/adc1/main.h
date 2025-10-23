#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "analog/adc1/basic.h"

extern ADC_HandleTypeDef hadc1;

Result adc_renew(ADC_OWN *adc, float32_t *current);
