#include "main/main.h"
#include "main/tim.h"
#include "motor/main.h"

inline void INIT_OWN(void)
{
    INIT_OWN_TIM();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (
           (GPIO_Pin == motor_h.const_h.Hall_GPIO_Pin_x[0])
        || (GPIO_Pin == motor_h.const_h.Hall_GPIO_Pin_x[1])
        || (GPIO_Pin == motor_h.const_h.Hall_GPIO_Pin_x[2])
    ) {
        motor_hall_exti(&motor_h);
    }
}

void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim)
{
    if (htim == motor_h.const_h.SPD_htimx)
    {
        motor_stop_trigger(&motor_h);
    }
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == motor_h.adc_a->const_h.hadcx)
    {
        motor_pwm_pulse(&motor_h);
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
