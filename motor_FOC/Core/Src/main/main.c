#include "main/main.h"
#include "main/tim.h"
#include "motor/main.h"

void INIT_OWN(void)
{
    tim_clk_APB1 = HAL_RCC_GetPCLK1Freq();
    tim_clk_APB2 = HAL_RCC_GetPCLK2Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) tim_clk_APB1 *= 2U;
    if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1) tim_clk_APB2 *= 2U;
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

inline void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim)
{
    if (htim == motor_h.const_h.TIM_htimx)
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
