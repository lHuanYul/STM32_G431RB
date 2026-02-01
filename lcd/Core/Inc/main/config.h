/*
#include "main/config.h"
*/
#pragma once
#include "HY_MOD/main/basic.h"

// ! SYSTEM config, Change CAREFULLY --------------------
// ! Also CHECK ALL basic.c file
#include "arm_math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "stm32g431xx.h"
#include "stm32g4xx_hal.h"

#define STM32_DEVICE
#define STM32G431RB
#define MCU_LCD
#include "HY_MOD/cmds.h"
#define FDCAN_FIFO0_FILTER0_ID_MIN  0x148
#define FDCAN_FIFO0_FILTER0_ID_MAX  0x14F
#define FDCAN_FIFO1_FILTER0_ID_MIN  0x140
#define FDCAN_FIFO1_FILTER0_ID_MAX  0x147
// #define FDCAN_FIFO1_FILTER0_ID_MIN  0x000
// #define FDCAN_FIFO1_FILTER0_ID_MAX  0x7FF

#define BOARD_LED_TOGGLE    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
#define BOARD_LED_ON        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define BOARD_LED_OFF       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)

#define TIM_DTG_200ns   0x22
#define TIM_DTG_300ns   0x33
#define TIM_DTG_500ns   0x55
#define TIM_DTG_1us     0x95

// #define HY_MOD_STM32_FDCAN
#define FDCAN_STD_FILTER_NBR    2
// #define FDCAN_RX0_NBR           16
// #define FDCAN_RX1_NBR           16
// #define FDCAN_RX_BUF_NBR        0
// #define FDCAN_TX_BUF_NBR        4
// #define FDCAN_TX_FIFO_NBR       16
// #define FDCAN_TX_EVT_NBR        (FDCAN_TX_BUF_NBR+FDCAN_TX_FIFO_NBR)
#define FDCAN_PKT_LEN           8
#define FDCAN_PKT_POOL_CAP      32
#define FDCAN_TRSM_BUF_CAP      10
#define FDCAN_RECV_BUF_CAP      10

#define HY_MOD_STM32_JSON
#define HY_MOD_STM32_SPI_JSON
#define JSON_PKT_LEN            1024
#define JSON_PKT_POOL_CAP       10
#define JSON_TRSM_BUF_CAP       4
#define JSON_RECV_BUF_CAP       4

#define HY_MOD_STM32_LCD_1INCH47
#define TIM16_PSC       0
#define TIM16_ARR       800 // Max: 65535

#define HY_MOD_STM32_YSM3

// ! Also CHECK ALL basic.c file
// ! SYSTEM config END ------------------------------
