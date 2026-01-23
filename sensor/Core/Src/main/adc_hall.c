#include "main/adc_hall.h"
#include "HY_MOD/adc/main.h"
#include "adc.h"

AdcHallParameter adchall_direction = {
    .adc = {
        .const_h = {
            .hadcx = &hadc1,
            // PC0
            .rankx = 0,
        },
    },
    .gate = 1780,
    .dbg_min = UINT16_MAX,
};

AdcHallParameter adchall_track_left = {
    .adc = {
        .const_h = {
            .hadcx = &hadc1,
            // PC1
            .rankx = 2,
        },
    },
    .gate = 1860,
    .dbg_min = UINT16_MAX,
};

AdcHallParameter adchall_track_right = {
    .adc = {
        .const_h = {
            .hadcx = &hadc1,
            // PC2
            .rankx = 1,
        },
    },
    .gate = 1850,
    .dbg_min = UINT16_MAX,
};

AdcHallParameter adchall_node = {
    .adc = {
        .const_h = {
            .hadcx = &hadc1,
            .rankx = 3,
        },
    },
    .gate = 1850,
    .dbg_min = UINT16_MAX,
};

bool adc_fdcan_enable = 0;
bool adc_fdcan_send = 0;

// 2100 - 1100
// static uint16_t adc_cnt[ADC_ACC_CNT_MAX - ADC_ACC_CNT_MIN] = {0};
// static void middle(const AdcParameter *adc, float32_t *store)
// {
//     memset(adc_cnt, 0, sizeof(adc_cnt));
//     uint16_t i, value;
//     for (i = 0; i < ADC_NEED_LEN; i++)
//     {
//         value = ADC_Values[i * ADC_COUNT + adc->const_h.rankx];
//         if(value < ADC_ACC_CNT_MIN || value >= ADC_ACC_CNT_MAX) continue;
//         adc_cnt[value - ADC_ACC_CNT_MIN]++;
//     }
//     uint16_t acc = 0;
//     for (i = 0; i < ADC_ACC_CNT_MAX - ADC_ACC_CNT_MIN; i++)
//     {
//         acc += adc_cnt[i];
//         if (acc > ADC_NEED_LEN / 2)
//         {
//             *store = i + ADC_ACC_CNT_MIN;
//             break;
//         }
//     }
// }

void StartAdcTask(void *argument)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    for(;;)
    {
        adc_hal_start_dma(&adchall_direction.adc);
        if (adc_fdcan_enable) adc_fdcan_send = 1;
        osDelay(100);
    }
}
