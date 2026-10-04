#include "main/main.h"
#include "tim.h"
#include "adc.h"
#include "HY_MOD/main/tim.h"
#include "HY_MOD/motor/basic.h"

MotorParameter motor_h = {
    .system.const_h = {
        .model = &MOTOR_MODEL,
        .PWM_htimx          = &htim1,
        .PWM_tim_clk        = &tim_clk_APB2,
        .PWM_u = {
            // .pwm_gpio = { .GPIOx = GPIOA, .Pin = GPIO_PIN_8  },
            .pwmn_gpio = { .GPIOx = GPIOB, .Pin = GPIO_PIN_13 },
            .pwm_ch = TIM_CHANNEL_1,
            .pwmn_mode = {
                .MODEx = GPIO_MODER_MODE13,
                .MODEx_0 = GPIO_MODER_MODE13_0,
                .MODEx_1 = GPIO_MODER_MODE13_1,
            },
        },
        .PWM_v = {
            // .pwm_gpio = { .GPIOx = GPIOA, .Pin = GPIO_PIN_9  },
            .pwmn_gpio = { .GPIOx = GPIOB, .Pin = GPIO_PIN_14 },
            .pwm_ch = TIM_CHANNEL_2,
            .pwmn_mode = {
                .MODEx = GPIO_MODER_MODE14,
                .MODEx_0 = GPIO_MODER_MODE14_0,
                .MODEx_1 = GPIO_MODER_MODE14_1,
            },
        },
        .PWM_w = {
            // .pwm_gpio = { .GPIOx = GPIOA, .Pin = GPIO_PIN_10 },
            .pwmn_gpio = { .GPIOx = GPIOB, .Pin = GPIO_PIN_15 },
            .pwm_ch = TIM_CHANNEL_3,
            .pwmn_mode = {
                .MODEx = GPIO_MODER_MODE15,
                .MODEx_0 = GPIO_MODER_MODE15_0,
                .MODEx_1 = GPIO_MODER_MODE15_1,
            },
        },
        .PWM_mid_ch = TIM_CHANNEL_4,

        .Hall_htimx     = &htim2,
        .Hall_tim_clk   = &tim_clk_APB1,
        .Hall_a = {
            // .tim_ch = TIM_CHANNEL_1,
            .gpio = { .GPIOx = GPIOA, .Pin = GPIO_PIN_0  },
        },
        .Hall_b = {
            // .tim_ch = TIM_CHANNEL_2,
            .gpio = { .GPIOx = GPIOA, .Pin = GPIO_PIN_1  },
        },
        .Hall_c = {
            // .tim_ch = TIM_CHANNEL_3,
            .gpio = { .GPIOx = GPIOB, .Pin = GPIO_PIN_10 },
        },
        .Hall_tim_val_ch = TIM_CHANNEL_1,
        .Hall_Active_ch = HAL_TIM_ACTIVE_CHANNEL_1,
    },
    .rotor = {
        .overflow = 170000000,
    },
    .speed.save_stop_omega = 1.0f,
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
