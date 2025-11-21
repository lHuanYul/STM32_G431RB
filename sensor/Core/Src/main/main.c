#include "main/main.h"
#include "main/tim.h"
#include "analog/adc1/main.h"

inline void INIT_OWN(void)
{
    INIT_OWN_TIM();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim)
{
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == adchall_direction.const_h.hadcx)
    {
        adc_update(&adchall_direction);
        adc_update(&adchall_track_left);
        adc_update(&adchall_track_right);
        adc_update(&adchall_node);
    }
}

#define DEFALT_TASK_DELAY_MS 50
uint32_t defalt_running;
void StartDefaultTask(void *argument)
{
    const uint32_t osPeriod = pdMS_TO_TICKS(DEFALT_TASK_DELAY_MS);
    uint32_t next_wake = osKernelGetTickCount() + osPeriod;
    for(;;)
    {
        defalt_running = HAL_GetTick();
        osDelayUntil(next_wake);
        next_wake += osPeriod;
    }
    osThreadExit();
}
