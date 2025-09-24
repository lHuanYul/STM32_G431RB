#include "analog/adc1/main.h"
#include "adc.h"

static uint16_t ADC_Values[ADC_COUNT * ADC_NEED_LEN] = {0};

static uint16_t adc_cnt[2560] = {0};

Result renew_adc(uint16_t adc_id, volatile uint16_t *adc_store)
{
    memset(adc_cnt, 0, sizeof(adc_cnt));
    uint16_t i, k;
    for (i = 0; i < ADC_NEED_LEN; i++)
    {
        k = ADC_Values[i * ADC_COUNT + adc_id];
        if(k > 2559) continue;
        adc_cnt[k]++;
    }
    const uint16_t target = (ADC_NEED_LEN-1)/2;
    uint16_t acc = 0;
    for (i = 500; i < 2500; i++)
    {
        acc += adc_cnt[i];
        if (acc > target)
        {
            *adc_store = i;
            break;
        }
    }
    return RESULT_OK(NULL);
}

void StartAdcTask(void *argument)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Values, ADC_COUNT * ADC_NEED_LEN);
    StopTask();
}
