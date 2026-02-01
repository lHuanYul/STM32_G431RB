#include "main/main.h"
#include "HY_MOD/main/tim.h"
#include "HY_MOD/motor/main.h"

inline void INIT_OWN(void)
{
    INIT_OWN_TIM();
}

#include "HY_MOD/motor/callback.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (
           (GPIO_Pin == motor_h.const_h.Hall_GPIO_Pin_x[0])
        || (GPIO_Pin == motor_h.const_h.Hall_GPIO_Pin_x[1])
        || (GPIO_Pin == motor_h.const_h.Hall_GPIO_Pin_x[2])
    ) {
        motor_hall_exti_cb(&motor_h);
    }
}

void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim)
{
    if (htim == motor_h.const_h.SPD_htimx)
    {
        motor_stop_cb(&motor_h);
    }
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == motor_h.adc_a->adc.hadcx)
    {
        motor_pwm_cb(&motor_h);
    }
}

#include "main/fdcan.h"
#include "HY_MOD/fdcan/callback.h"

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    fdcan_error_status_cb(&fdcan_h, hfdcan, ErrorStatusITs);
}

void HAL_FDCAN_TxEventFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxEventFifoITs)
{
    fdcan_tx_fifo_cb(&fdcan_h, hfdcan, TxEventFifoITs);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    fdcan_rx_fifo0_cb(&fdcan_h, hfdcan, RxFifo0ITs, &fdcan_pkt_pool, &fdcan_recv_pkt_buf);
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    fdcan_rx_fifo1_cb(&fdcan_h, hfdcan, RxFifo1ITs);
}

#include "HY_MOD/motor/main.h"
#define DEFALT_TASK_DELAY_MS 50
uint32_t defalt_running;
void StartDefaultTask(void *argument)
{
    motor_setup(&motor_h);
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
