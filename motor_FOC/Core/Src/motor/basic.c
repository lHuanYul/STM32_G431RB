#include "motor/basic.h"
#include "tim.h"

const uint8_t hall_seq_clw[8] = {0, 3, 6, 2, 5, 1, 4, 0};  // 正轉
const uint8_t hall_seq_ccw[8] = {0, 5, 3, 1, 6, 4, 2, 0};  // 反轉

// Commutation right_SEQUENCE for 120 degree control
#define HIGH_PASS   1
#define NONE_PASS   0
#define LOW_PASS   -1
static const int8_t motor_seq_map[6][3] = {
    { HIGH_PASS, LOW_PASS,  NONE_PASS },
    { HIGH_PASS, NONE_PASS, LOW_PASS  },
    { NONE_PASS, HIGH_PASS, LOW_PASS  },
    { LOW_PASS,  HIGH_PASS, NONE_PASS },
    { LOW_PASS,  NONE_PASS, HIGH_PASS },
    { NONE_PASS, LOW_PASS,  HIGH_PASS }
};
static const uint8_t motor_seq_index[] = {0xFF, 5, 3, 4, 1, 0, 2, 0xFF};

MotorParameter motor_0 = {
    .const_h = {
        .adc_u_id = 0,
        .adc_v_id = 1,
        .adc_w_id = 2,
        .Hall_GPIOx         = { GPIOC,       GPIOC,       GPIOC       },
        .Hall_GPIO_Pin_x    = { GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_10 },
        // H: PC0 PC1 PC2 // L: PB13 PB14 PB15
        .htimx          = { &htim1,        &htim1,        &htim1        },
        .TIM_CHANNEL_x  = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 },
        // 120 deg control L
        .Coil_GPIOx         = { GPIOB,       GPIOB,       GPIOB       },
        .Coil_GPIO_Pin_x    = { GPIO_PIN_5,  GPIO_PIN_4,  GPIO_PIN_10 },
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

Result hall_to_angle(uint8_t hall, volatile uint16_t *angle)
{
    switch(hall)
    {
        case 6:
        {
            *angle = 0;
            break;
        }
        case 2:
        {
            *angle = 60;
            break;
        }
        case 3:
        {
            *angle = 120;
            break;
        }
        case 1:
        {
            *angle = 180;
            break;
        }
        case 5:
        {
            *angle = 240;
            break;
        }
        case 4:
        {
            *angle = 300;
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
    return RESULT_OK(NULL);
}

// 120 deg cntrol
void step_commutate_120(const MotorParameter *motor)
{
    const MotorConst* const_h = &motor->const_h;
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        switch (motor_seq_map[motor_seq_index[motor->exti_hall_curt]][i])
        {
            case HIGH_PASS:
            {
                __HAL_TIM_SET_COMPARE(const_h->htimx[i], const_h->TIM_CHANNEL_x[i], (uint32_t)((float)TIM1_ARR * motor->pwm_duty_u));
                HAL_GPIO_WritePin(const_h->Coil_GPIOx[i], const_h->Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
                break;
            }
            case LOW_PASS:
            {
                __HAL_TIM_SET_COMPARE(const_h->htimx[i], const_h->TIM_CHANNEL_x[i], 0);
                HAL_GPIO_WritePin(const_h->Coil_GPIOx[i], const_h->Coil_GPIO_Pin_x[i],  GPIO_PIN_SET);
                break;
            }
            default:
            {
                __HAL_TIM_SET_COMPARE(const_h->htimx[i], const_h->TIM_CHANNEL_x[i], 0);
                HAL_GPIO_WritePin(const_h->Coil_GPIOx[i], const_h->Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
                break;
            }
        }
    }
}
