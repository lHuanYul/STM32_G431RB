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
#define MCU_MOTOR_CTRL

#define HY_MOD_STM32_MOTOR
// #define MOTOR_FOC_SPIN_DEBUG
#define MOTOR_AUTO_SPIN
#include "HY_MOD/motor/motor_parameter.h"
#define MOTOR_POLE      MOTOR_VEHICLE_POLE
#define MOTOR_GEAR      MOTOR_VEHICLE_GEAR
#define MOTOR_FOC_ANGLE MOTOR_42BLF01_ANGLE
#define MOTOR_RPM_CNT           6
#define MOTOR_HISTORY_LEN       100
// 20kHz 50us pwm
#define TIM1_PSC        0
#define TIM1_ARR        4250 // Max: 65535
// Motor Time Calculate
#define TIM2_PSC        0
#define TIM2_ARR        17000000*5 // Max: 4294967295
// 20kHz 50us 20kHzIT
#define TIM3_PSC        0
#define TIM3_ARR        8500 // Max: 65535
// PWM Dead Time
#define TIM_DTG_200ns   0x22
#define TIM_DTG_300ns   0x33
#define TIM_DTG_500ns   0x55
#define TIM_DTG_1us     0x95

#define HY_MOD_STM32_ADC_CURRENT
#define ADC_COUNT       4
#define ADC_NEED_LEN    5 // 511
#define ADC_TO_VOL      (3.3f/4095.0f)
#define ADC_VOL_SEP     (2.0f/3.0f)

#define HY_MOD_STM32_FDCAN
#include "HY_MOD/cmds.h"
#define FDCAN_FIFO0_FILTER0_ID_MIN  0x148
#define FDCAN_FIFO0_FILTER0_ID_MAX  0x14F
#define FDCAN_FIFO1_FILTER0_ID_MIN  0x140
#define FDCAN_FIFO1_FILTER0_ID_MAX  0x147
// #define FDCAN_FIFO1_FILTER0_ID_MIN  0x000
// #define FDCAN_FIFO1_FILTER0_ID_MAX  0x7FF
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
