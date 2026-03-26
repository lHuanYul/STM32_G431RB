#pragma once

#include "main/config.h"

#include "HY_MOD/packet/fdcan.h"
extern FdcanPktPool fdcan_pkt_pool;
extern FdcanPktBuf fdcan_trsm_pkt_buf;
extern FdcanPktBuf fdcan_recv_pkt_buf;
#include "HY_MOD/fdcan/basic.h"
extern FdcanParametar fdcan_h;
#include "HY_MOD/motor/basic.h"
extern MotorParameter motor_h;

void BUTTON_OWN(void);
void MY_INIT_OWN(void);
void HAL_TIM_PeriodElapsedCallback_OWN(TIM_HandleTypeDef *htim);
