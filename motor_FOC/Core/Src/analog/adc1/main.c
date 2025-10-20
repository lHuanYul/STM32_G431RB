#include "analog/adc1/main.h"
#include "adc.h"

static uint16_t ADC_Values[ADC_COUNT * ADC_NEED_LEN] = {0};

// static uint16_t adc_cnt[2560] = {0};
// static UNUSED_FNC Result middle(uint8_t adc_id, uint16_t *adc_store)
// {
//     memset(adc_cnt, 0, sizeof(adc_cnt));
//     uint16_t i, val;
//     for (i = 0; i < ADC_NEED_LEN; i++)
//     {
//         val = ADC_Values[i * ADC_COUNT + adc_id];
//         if(val > 2559) continue;
//         adc_cnt[val]++;
//     }
//     const uint16_t target = (ADC_NEED_LEN-1)/2;
//     uint16_t acc = 0;
//     for (i = 500; i < 2500; i++)
//     {
//         acc += adc_cnt[i];
//         if (acc > target)
//         {
//             *adc_store = i;
//             break;
//         }
//     }
//     return RESULT_OK(NULL);
// }

static void average(uint8_t adc_id, float32_t *adc_store)
{
    uint16_t i;
    float32_t total = 0;
    for (i = 0; i < ADC_NEED_LEN; i++)
    {
        total += ADC_Values[i * ADC_COUNT + adc_id];
    }
    *adc_store = total / (float32_t)ADC_NEED_LEN;
}

#define ALPHA 0.2f  
static inline void iir(uint8_t adc_id, float32_t *adc_store)
{
    *adc_store += ALPHA * ((float32_t)ADC_Values[adc_id] - *adc_store);
}

Result adc_renew(ADC_OWN *adc, float32_t *current)
{
    average(adc->const_h.id, &adc->value);
    *current = (adc->value - adc->zero) * adc->current_trs;
    return RESULT_OK(NULL);
}

#define ADC_TRANS (3.3f / 4095.0f * 2.0f / 3.0f)
static void adc_init(ADC_OWN *adc)
{
    adc->current_trs = ADC_TRANS / adc->const_h.sensitive;
    average(adc->const_h.id, &adc->value);
    adc->zero = adc->value;
}

void StartAdcTask(void *argument)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Values, ADC_COUNT * ADC_NEED_LEN);
    osDelay(1);
    adc_init(&adc_u);
    adc_init(&adc_v);
    adc_init(&adc_w);
    StopTask();
}
