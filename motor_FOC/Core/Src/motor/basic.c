#include "motor/basic.h"
#include "tim.h"

const uint8_t hall_seq_clw[8] = {255, 3, 6, 2, 5, 1, 4, 255};  // 正轉
const uint8_t hall_seq_ccw[8] = {255, 5, 3, 1, 6, 4, 2, 255};  // 反轉

const int8_t motor_sequence[6][3] = {
  { MS_HIGH_PASS, MS_LOW_PASS,  MS_NONE_PASS },
  { MS_HIGH_PASS, MS_NONE_PASS, MS_LOW_PASS  },
  { MS_NONE_PASS, MS_HIGH_PASS, MS_LOW_PASS  },
  { MS_LOW_PASS,  MS_HIGH_PASS, MS_NONE_PASS },
  { MS_LOW_PASS,  MS_NONE_PASS, MS_HIGH_PASS },
  { MS_NONE_PASS, MS_LOW_PASS,  MS_HIGH_PASS }
};

MotorParameter motor_0 = {
    .const_h = {
        .adc_u_id = 0,
        .adc_v_id = 1,
        .adc_w_id = 2,
        .Hall_GPIOx         = { GPIOC,       GPIOC,       GPIOC     },
        .Hall_GPIO_Pin_x    = { GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12},
        // PC0 PC1 PC2
        .PWM_htimx          = { &htim1,        &htim1,        &htim1        },
        .PWM_TIM_CHANNEL_x  = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 },
        // .Coil_GPIOx         = { GPIOB,       GPIOB,       GPIOB       },
        // .Coil_GPIO_Pin_x    = { GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 },
        .ELE_htimx = &htim2,
    },
    .pi_speed = {
        .Kp = 0.000025f,
        .Ki = 0.002f,
        .Umax = 0.1f,
        .Umin = -0.1f,
        .w1 = 1.0f,
    },
    .pi_Iq = {
        .Ref = 0.5f,
        .Kp = 0.3f,
        .Ki = 0.001f,
        .Umax = 0.1f,
        .Umin = -0.1f,
        .w1 = 1.0f,
    },
    .pi_Id = {
        .Kp = 0.2f,
        .Ki = 0.001f,
        .Umax = 0.01f,
        .Umin = -0.01f,
        .w1 = 1.0f,
    },
};

// void step_commutate(const MotorParameter *motor)
// {
//     const MotorConst* const_h = &motor->const_h;
//     uint8_t i;
//     for (i = 0; i < 3; i++)
//     {
//         switch (motor_sequence[motor->gpio_hall_current][i])
//         {
//             case MS_HIGH_PASS:
//             {
//                 __HAL_TIM_SET_COMPARE(const_h->PWM_htimx[i], const_h->PWM_TIM_CHANNEL_x[i], motor->pwm_compare);
//                 HAL_GPIO_WritePin(const_h->Coil_GPIOx[i], const_h->Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
//                 break;
//             }
//             case MS_NONE_PASS:
//             {
//                 __HAL_TIM_SET_COMPARE(const_h->PWM_htimx[i], const_h->PWM_TIM_CHANNEL_x[i], 0);
//                 HAL_GPIO_WritePin(const_h->Coil_GPIOx[i], const_h->Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
//                 break;
//             }
//             case MS_LOW_PASS:
//             {
//                 __HAL_TIM_SET_COMPARE(const_h->PWM_htimx[i], const_h->PWM_TIM_CHANNEL_x[i], 0);
//                 HAL_GPIO_WritePin(const_h->Coil_GPIOx[i], const_h->Coil_GPIO_Pin_x[i],  GPIO_PIN_SET);
//                 break;
//             }
//             default:
//             {
//                 Error_Handler();
//                 break;
//             }
//         }
//     }
// }
