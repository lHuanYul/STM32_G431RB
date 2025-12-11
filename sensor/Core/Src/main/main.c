#include "main/main.h"
#include "main/tim.h"
#include "analog/adc1/main.h"
#include "us_sensor/main.h"

inline void INIT_OWN(void)
{
    INIT_OWN_TIM();
}

int text_exit = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    text_exit++;
    if (GPIO_Pin == us_sensor_head.const_h.echo_GPIO_Pin_x) // 6
    {
        us_sensor_echo();
    }
}

int text_over = 0;
void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim)
{
    text_over++;
    if (htim == us_sensor_head.const_h.htimx)
    {
        us_sensor_overflow();
    }
}

int text_tim = 0;
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    text_tim++;
    if (
        htim == us_sensor_head.const_h.htimx &&
        htim->Channel == us_sensor_head.const_h.HAL_TIM_ACTIVE_CHANNEL_x
    ) {
        us_sensor_tri_off();
    }
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
