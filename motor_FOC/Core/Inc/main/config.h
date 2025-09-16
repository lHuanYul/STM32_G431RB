/*
#include "main/config.h"
*/
#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "stm32g431xx.h"
#include "stm32g4xx_hal.h"
#include "main.h"

#define UNUSED_FNC          __attribute__((unused))
#define BOARD_LED_TOGGLE    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
#define BOARD_LED_ON        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define BOARD_LED_OFF       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
#define ITS_CHECK(its, tag) (((its) & (tag)) != RESET)

// ! SYSTEM config, Change CAREFULLY --------------------
#define STM32_DEVICE

#define MOTOR_POLE            20

#define ADC_COUNT       3
#define ADC_NEED_LEN    511

#define TIM1_PSC        16
#define TIM1_ARR        499 // 65535
#define TIM2_PSC        16
#define TIM2_ARR        999 // 4294967295

#define TIM_DTG_200ns   0x22
#define TIM_DTG_300ns   0x33
#define TIM_DTG_500ns   0x55
#define TIM_DTG_1us     0x95

#define FDCAN_STD_FILTER_NBR    2
// #define FDCAN_RX0_NBR           16
// #define FDCAN_RX1_NBR           16
// #define FDCAN_RX_BUF_NBR        0
// #define FDCAN_TX_BUF_NBR        4
// #define FDCAN_TX_FIFO_NBR       16
// #define FDCAN_TX_EVT_NBR        (FDCAN_TX_BUF_NBR+FDCAN_TX_FIFO_NBR)
#define FDCAN_FILTER0_ID_MIN    0x020
#define FDCAN_FILTER0_ID_MAX    0x021
#define FDCAN_FILTER1_ID_MIN    0x022
#define FDCAN_FILTER1_ID_MAX    0x023
#define FDCAN_TEST_ID           0x001
#define FDCAN_DATA_ID           0x011
#define FDCAN_PKT_LEN           8
#define FDCAN_PKT_POOL_CAP      32
#define FDCAN_TRSM_BUF_CAP      10
#define FDCAN_RECV_BUF_CAP      10

// ! SYSTEM config END ------------------------------
