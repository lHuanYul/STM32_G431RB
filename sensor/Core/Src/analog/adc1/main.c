#include "analog/adc1/main.h"
#include "adc.h"
#include "main/config.h"

static uint16_t ADC_Values[ADC_COUNT] = {0};

bool adc_fdcan_enable = 0;
bool adc_fdcan_send = 0;

ADC_PARAMETER adchall_direction = {
    .const_h = {
        .hadcx = &hadc1,
        // PC0
        .rankx = 0,
    },
    .gate = 1780,
    .dbg_min = UINT16_MAX,
};

ADC_PARAMETER adchall_track_left = {
    .const_h = {
        .hadcx = &hadc1,
        // PC1
        .rankx = 2,
    },
    .gate = 1860,
    .dbg_min = UINT16_MAX,
};

ADC_PARAMETER adchall_track_right = {
    .const_h = {
        .hadcx = &hadc1,
        // PC2
        .rankx = 1,
    },
    .gate = 1850,
    .dbg_min = UINT16_MAX,
};

ADC_PARAMETER adchall_node = {
    .const_h = {
        .hadcx = &hadc1,
        .rankx = 3,
    },
    .gate = 1850,
    .dbg_min = UINT16_MAX,
};

static void max_min (ADC_PARAMETER* adc)
{
    adc->dbg_tim++;
    if (adc->dbg_tim >= 100)
    {
        adc->dbg_tim = 0;
        adc->dbg_max = 0;
        adc->dbg_min = 4095;
    }

    if(adc->value > adc->dbg_max) adc->dbg_max = adc->value;
    else if(adc->value < adc->dbg_min && adc->value > 800) adc->dbg_min = adc->value;
}

static void average(const ADC_PARAMETER *adc, float32_t *store)
{
    uint16_t i;
    float32_t total = 0;
    for (i = 0; i < ADC_NEED_LEN; i++)
    {
        total += ADC_Values[i * ADC_COUNT + adc->const_h.rankx];
    }
    *store = total / (float32_t)ADC_NEED_LEN;
}

// 2100 - 1100
static uint16_t adc_cnt[ADC_ACC_CNT_MAX - ADC_ACC_CNT_MIN] = {0};
static void middle(const ADC_PARAMETER *adc, float32_t *store)
{
    memset(adc_cnt, 0, sizeof(adc_cnt));
    uint16_t i, value;
    for (i = 0; i < ADC_NEED_LEN; i++)
    {
        value = ADC_Values[i * ADC_COUNT + adc->const_h.rankx];
        if(value < ADC_ACC_CNT_MIN || value >= ADC_ACC_CNT_MAX) continue;
        adc_cnt[value - ADC_ACC_CNT_MIN]++;
    }
    uint16_t acc = 0;
    for (i = 0; i < ADC_ACC_CNT_MAX - ADC_ACC_CNT_MIN; i++)
    {
        acc += adc_cnt[i];
        if (acc > ADC_NEED_LEN / 2)
        {
            *store = i + ADC_ACC_CNT_MIN;
            break;
        }
    }
}

#define ALPHA 0.1f
static void iir(const ADC_PARAMETER *adc, float32_t *store)
{
    float32_t avg;
    average(adc, &avg);
    *store += ALPHA * (avg - *store);
}

void adc_update(ADC_PARAMETER* adc)
{
    // middle(adc, &adc->value);
    adc->value = ADC_Values[adc->const_h.rankx];
    if (adc->value <= adc->gate) adc->state = ADC_HALL_STATE_ON_MAG;
    else adc->state = ADC_HALL_STATE_NONE;
    max_min(adc);
}

void StartAdcTask(void *argument)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    for(;;)
    {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Values, ADC_COUNT);
        if (adc_fdcan_enable) adc_fdcan_send = 1;
        osDelay(100);
    }
}
