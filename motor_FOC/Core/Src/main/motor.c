#include "main/main.h"
#include "tim.h"
#include "HY_MOD/main/tim.h"

MotorParameter motor_h = {
    .const_h = {
        // PA8      ------> TIM1_CH1
        // PA9      ------> TIM1_CH2
        // PA10     ------> TIM1_CH3
        // PB13     ------> TIM1_CH1N
        // PB14     ------> TIM1_CH2N
        // PB15     ------> TIM1_CH3N
        .PWM_htimx          = &htim1,
        .PWM_tim_clk        = &tim_clk_APB2,
        .PWM_TIM_CH_x  = {
            .u = TIM_CHANNEL_1,
            .v = TIM_CHANNEL_2,
            .w = TIM_CHANNEL_3,
            .mid = TIM_CHANNEL_4,
        },
        .PWMN_GPIO = {
            .u = { .GPIOx = GPIOB, .Pin = GPIO_PIN_13 },
            .v = { .GPIOx = GPIOB, .Pin = GPIO_PIN_14 },
            .w = { .GPIOx = GPIOB, .Pin = GPIO_PIN_15 },
        },
        .PWMN_GPIO_set = {
            .u = {
                .MODEx = GPIO_MODER_MODE13,
                .MODEx_0 = GPIO_MODER_MODE13_0,
                .MODEx_1 = GPIO_MODER_MODE13_1,
            },
            .v = {
                .MODEx = GPIO_MODER_MODE14,
                .MODEx_0 = GPIO_MODER_MODE14_0,
                .MODEx_1 = GPIO_MODER_MODE14_1,
            },
            .w = {
                .MODEx = GPIO_MODER_MODE15,
                .MODEx_0 = GPIO_MODER_MODE15_0,
                .MODEx_1 = GPIO_MODER_MODE15_1,
            },
        },
        // PA0     ------> TIM2_CH1
        // PA1     ------> TIM2_CH2
        // PB10     ------> TIM2_CH3
        .Hall_htimx     = &htim2,
        .Hall_tim_clk   = &tim_clk_APB1,
        .Hall_GPIO = {
            .u = { .GPIOx = GPIOA, .Pin = GPIO_PIN_0  },
            .v = { .GPIOx = GPIOA, .Pin = GPIO_PIN_1  },
            .w = { .GPIOx = GPIOB, .Pin = GPIO_PIN_10 },
        },
        // 42BLF01
        .rated_current  = MOTOR_42BLF01_RATED_CURRENT,
        .peak_current   = MOTOR_42BLF01_PEAK_CURRENT,
    },
    .init_cnt = 20000,
    .hall_h.auto_spin = 4,
    .tfm_h.duty_Iq = 1.0f,
    .rpm_h.save_stop_val = 10.0f,
    .deg_h = {
        .pi_rpm = {
            .Kp = 0.005f,
            .Ki = 0.02f,
            .max = 1.0f,
            .min = -1.0f,
        },
    },
    .foc_h = {
        // Yellow Green Blue
        .adc_h = {
            .u = &adc_current_h[0],
            .v = &adc_current_h[1],
            .w = &adc_current_h[2],
        },
        .pi_rpm = {
            .Kp = 0.005f,
            .Ki = 0.02f,
            .max = 1.0f,
            .min = -1.0f,
        },
        .pi_Id_h = {
            .max =  ONE_DIV_SQRT3,
            .min = -ONE_DIV_SQRT3,
        },
        .pi_Iq_h = {},
    },
};
