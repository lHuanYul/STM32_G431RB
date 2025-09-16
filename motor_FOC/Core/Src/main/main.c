#include "main/main.h"
#include "motor/hall.h"
#include "motor/pwm.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (
           (GPIO_Pin == motor_0.const_h.Hall_GPIO_Pin_x[0])
        || (GPIO_Pin == motor_0.const_h.Hall_GPIO_Pin_x[1])
        || (GPIO_Pin == motor_0.const_h.Hall_GPIO_Pin_x[2])
    ) {
        motor_hall_exti(&motor_0);
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == motor_0.const_h.PWM_htimx[0])
    {
        motor_pwm_pulse(&motor_0);
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
