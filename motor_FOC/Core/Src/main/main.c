#include "main/main.h"
#include "HY_MOD/main/tim.h"
#include "HY_MOD/motor/main.h"

// overwrite default weak function
void motor_start_spin(MotorParameter *motor)
{
	motor_set_rotor_mode(motor, MOTOR_SENSOR_HALL_EXTI);
	motor_set_ctrl_mode(motor, MOTOR_CTRL_120_DUTY);
	motor_set_rotate_mode(motor, MOTOR_ROTATE_NORMAL);
	motor_set_speed(motor, 0.2f);
}

#include "HY_MOD/adc/main.h"

// void EXTI15_10_IRQHandler(void)
inline void MY_Button(void)
{
    // adc_max_min_reset(&adc_current_h[0].basic);
    // motor_h.rotor.vir_tri = 1;
}

#include "HY_MOD/motor/callback.h"
#include "HY_MOD/fdcan/callback.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    MOTOR_HAL_TIM_IC_CaptureCB_CALL(motor_h, htim);
}

void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim)
{
    MOTOR_HAL_TIM_PeriodElapsedCB_CALL(motor_h, htim);
    FDCAN_HAL_TIM_PeriodElapsedCB_CALL(fdcan_h, htim);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    MOTOR_HAL_ADCEx_InjectedConvCpltCB_CALL(motor_h, hadc);
}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    FDCAN_HAL_ErrorStatusCB_CALL(fdcan_h, hfdcan, ErrorStatusITs);
}

void HAL_FDCAN_TxEventFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxEventFifoITs)
{
    FDCAN_HAL_TxEventFifoCB_CALL(fdcan_h, hfdcan, TxEventFifoITs);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    FDCAN_HAL_RxFifo0CB_CALL(fdcan_h, hfdcan, RxFifo0ITs);
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    FDCAN_HAL_RxFifo1CB_CALL(fdcan_h, hfdcan, RxFifo1ITs);
}

#include "HY_MOD/fdcan/main.h"
#include "HY_MOD/motor/main.h"
#define DEFALT_TASK_DELAY_MS 50
uint32_t default_running;
uint32_t system_clk;
// int main(void)
void StartDefaultTask(void *argument)
{
    system_clk = HAL_RCC_GetHCLKFreq();
    motor_init(&motor_h);
    fdcan_setup(&fdcan_h);
    fdcan_tim_start(&fdcan_h);
    osThreadSetPriority(osThreadGetId(), osPriorityIdle);
    for(;;)
    {
        default_running = HAL_GetTick();
        fdcan_main(&fdcan_h);
        motor_main(&motor_h);
        osThreadYield();
    }
    osThreadExit();
}
