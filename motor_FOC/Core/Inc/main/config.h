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

#define UNUSED_FNC          __attribute__((unused))

// ! SYSTEM config, Change CAREFULLY --------------------
// ! Also CHECK ALL basic.c file
#include "arm_math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "stm32g431xx.h"
#include "stm32g4xx_hal.h"

#define STM32_DEVICE
#define STM32_G431RB
#define MCU_MOTOR_CTRL
// #define MOTOR_FOC_SPIN_DEBUG

#define ITS_CHECK(its, tag) (((its) & (tag)) != RESET)
#ifndef PI // 180 deg
    #define PI  3.14159265358979f
    // #define PI  3.14159265358979323846f 
#endif
#define PI_MUL_2        (2.0f * PI)     // 360 deg
#define PI_DIV_6        (PI / 6.0f)     // 30 deg
#define PI_DIV_3        (PI / 3.0f)     // 60 deg
#define PI_DIV_2        (PI / 2.0f)     // 90 deg
#define DEG_TO_RAD      (PI / 180.0f)
#define RAD_TO_DEG      (180.0f / PI)
#define DIV_1_3         (1.0f / 3.0f)
#define DIV_2_3         (2.0f / 3.0f)
#define ONE_DIV_SQRT3   0.577350269189626f
#define SQRT3_DIV_2     0.866025403784439f

#define BOARD_LED_TOGGLE    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
#define BOARD_LED_ON        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define BOARD_LED_OFF       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)

#define MOTOR_42BLF01_POLE      4
#define MOTOR_42BLF01_GEAR      4.4f
// 霍爾訊號與實際電角校正
// ? 霍爾超前實際為負
#define MOTOR_42BLF01_ANGLE     (PI_DIV_6*-5.0f) // 6 // -3/3 -4/2 -5/2 -6/6
#define MOTOR_STOP_TRI          50

#define ADC_COUNT       4
#define ADC_NEED_LEN    1 // 511
#define ADC_TO_VOL      (3.3f/4095.0f)
#define ADC_VOL_SEP     (2.0f/3.0f)

// Motor Core
// 20kHz 50us
#define TIM1_PSC        16
#define TIM1_ARR        500 // Max: 65535
// Motor Time Calculate
#define TIM2_PSC        0
#define TIM2_ARR        4294967295 // Max: 4294967295
// Programe Time Calculate
#define TIM3_PSC        16
#define TIM3_ARR        65535 // Max: 65535

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

// ! Also CHECK ALL basic.c file
// ! SYSTEM config END ------------------------------
