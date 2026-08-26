#include "main/main.h"
#include "tim.h"
#include "adc.h"
#include "HY_MOD/main/tim.h"

MotorParameter motor_h = {
    .const_h = {
        .model = &MOTOR_MODEL,
        // PA8  ------> TIM1_CH1
        // PA9  ------> TIM1_CH2
        // PA10 ------> TIM1_CH3
        // PB13 ------> TIM1_CH1N
        // PB14 ------> TIM1_CH2N
        // PB15 ------> TIM1_CH3N
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
        // PA0  ------> TIM2_CH1
        // PA1  ------> TIM2_CH2
        // PB10 ------> TIM2_CH3
        .Hall_htimx     = &htim2,
        .Hall_tim_clk   = &tim_clk_APB1,
        .Hall_GPIO = {
            .u = { .GPIOx = GPIOA, .Pin = GPIO_PIN_0  },
            .v = { .GPIOx = GPIOA, .Pin = GPIO_PIN_1  },
            .w = { .GPIOx = GPIOB, .Pin = GPIO_PIN_10 },
        },
    },
    .init_cnt = 20000,
    .rotor_h = {
        .virtual = 4,
    },
    .speed_h.save_stop_omega = 1.0f,
    // Yellow Green Blue (42BLF01)
    .adc_h = {
        .adc_ui = {
            .model = &ADC_MODEL_I,
            // ADC1 CH11 PB12 0.097
            .basic = {
                .hadcx = &hadc1,
                .rankx = ADC_INJECTED_RANK_1,
            },
        },
        .adc_vi = {
            .model = &ADC_MODEL_I,
            // ADC2 CH12 PB2
            .basic = {
                .hadcx = &hadc2,
                .rankx = ADC_INJECTED_RANK_1,
            },
        },
        .adc_wi = {
            .model = &ADC_MODEL_I,
            // ADC1 CH14 PB11
            .basic = {
                .hadcx = &hadc1,
                .rankx = ADC_INJECTED_RANK_2,
            },
        },
        .adc_uv = {
            .model = &ADC_MODEL_I,
            // ADC1 CH11 PB12 0.097
            .basic = {
                .hadcx = &hadc2,
                .rankx = ADC_INJECTED_RANK_2,
            },
        },
        .adc_vv = {
            .model = &ADC_MODEL_I,
            // ADC2 CH12 PB2
            .basic = {
                .hadcx = &hadc1,
                .rankx = ADC_INJECTED_RANK_3,
            },
        },
        .adc_wv = {
            .model = &ADC_MODEL_I,
            // ADC1 CH14 PB11
            .basic = {
                .hadcx = &hadc2,
                .rankx = ADC_INJECTED_RANK_3,
            },
        },
    },
    .deg_h = {
        // Setting in HY_MCU_MOD/motor/ctrl_deg.c
        // .pi_omega = {},
        .pi_current = {
            .Kp = 0.005f,
            .Ki = 0.02f,
            .max = 1.0f,
            .min = 0.0f,
        },
    },
    .foc_h = {
        // Setting in HY_MCU_MOD/motor/ctrl_foc.c
        // .pi_omega = {},
        // .pi_Id_h = {},
        // .pi_Iq_h = {},
    },
};
