#pragma once

#include "main/config.h"

#include "HY_MOD/fdcan/basic.h"
extern FdcanParametar fdcan_h;
#include "HY_MOD/motor/basic.h"
extern MotorParameter motor_h;

void BUTTON_OWN(void);
void MY_OTH_Init(void);
void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim);
