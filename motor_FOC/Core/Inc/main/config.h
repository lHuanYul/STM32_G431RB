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

#define ATTR_X(...) __attribute__((__VA_ARGS__))
#define ATTR_UNUSED __attribute__((unused))
#define ATTR_WEAK   __attribute__((weak))

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
#define ITS_CHECK(its, tag) (((its) & (tag)) != RESET)
#define BOARD_LED_TOGGLE HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
#define MCU_MOTOR_CTRL
#include "connectivity/cmds.h"
#define FDCAN_FIFO0_FILTER0_ID_MIN  0x100
#define FDCAN_FIFO0_FILTER0_ID_MAX  0x10F
// #define FDCAN_FIFO1_FILTER0_ID_MIN  0x140
// #define FDCAN_FIFO1_FILTER0_ID_MAX  0x14F
#define FDCAN_FIFO1_FILTER0_ID_MIN  0x000
#define FDCAN_FIFO1_FILTER0_ID_MAX  0x7FF

#define MOTOR_LEFT
#ifdef MOTOR_LEFT
// 739
#define FDCAN_WHEEL_FBK_ID          CAN_ID_WHEEL_LEFT_SPD_FBK
#define FDCAN_WHEEL_SET_ID          CAN_ID_WHEEL_LEFT_SET
#else
// 734
#define FDCAN_WHEEL_FBK_ID          CAN_ID_WHEEL_RIGHT_SPD_FBK
#define FDCAN_WHEEL_SET_ID          CAN_ID_WHEEL_RIGHT_SET
#endif
// #define MOTOR_FOC_SPIN_DEBUG

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
#define DIV_1_3         (1.0f / 3.0f)   // 1/3
#define DIV_2_3         (2.0f / 3.0f)   // 2/3
#define ONE_DIV_SQRT3   0.577350269189626f  // 1/(根號3)
#define SQRT3_DIV_2     0.866025403784439f  // (根號3)/2
#define RPM_TO_RAD      (PI_MUL_2 / 60.0f)  //

#define BOARD_LED_TOGGLE    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
#define BOARD_LED_ON        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define BOARD_LED_OFF       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)

#define MOTOR_42BLF01_POLE          8
#define MOTOR_42BLF01_GEAR          3.0f
#define MOTOR_42BLF01_RATED_CURRENT 1.9f
#define MOTOR_42BLF01_PEAK_CURRENT  5.7f
// 霍爾訊號與實際電角校正
// 霍爾超前實際為負
#define MOTOR_42BLF01_ANGLE     (PI_DIV_6*-5.0f) // 6 // -3/3 -4/2 -5/2 -6/6
#define MOTOR_RPM_CNT           6

#define ADC_COUNT       4
#define ADC_NEED_LEN    5 // 511
#define ADC_TO_VOL      (3.3f/4095.0f)
#define ADC_VOL_SEP     (2.0f/3.0f)

// Motor Core
// 20kHz 50us pwm
#define TIM1_PSC        0
#define TIM1_ARR        4250 // Max: 65535
// Motor Time Calculate
#define TIM2_PSC        0
#define TIM2_ARR        17000000*5 // Max: 4294967295
// 20kHz 50us 20kHzIT
#define TIM3_PSC        0
#define TIM3_ARR        8500 // Max: 65535

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
#define FDCAN_PKT_LEN           8
#define FDCAN_PKT_POOL_CAP      32
#define FDCAN_TRSM_BUF_CAP      10
#define FDCAN_RECV_BUF_CAP      10

// ! Also CHECK ALL basic.c file
// ! SYSTEM config END ------------------------------

typedef uint8_t Percentage;
