#pragma once

#include "main/config.h"

extern bool adc_fdcan_enable;
extern bool adc_fdcan_send;

typedef struct ADC_PARAMETER_CONST
{
    ADC_HandleTypeDef *hadcx;
    uint32_t rankx;
} ADC_PARAMETER_CONST;

typedef enum AdcState
{
    ADC_HALL_STATE_NONE,
    ADC_HALL_STATE_ON_MAG,
} AdcState;

typedef struct ADC_PARAMETER
{
    const ADC_PARAMETER_CONST const_h;
    uint16_t value;
    uint16_t gate;
    AdcState state;
    uint16_t dbg_max;
    uint16_t dbg_min;
    uint16_t dbg_tim;
} ADC_PARAMETER;

extern ADC_PARAMETER adchall_track_left;
extern ADC_PARAMETER adchall_track_right;
extern ADC_PARAMETER adchall_node;
extern ADC_PARAMETER adchall_direction;

void adc_update(ADC_PARAMETER* adc);
