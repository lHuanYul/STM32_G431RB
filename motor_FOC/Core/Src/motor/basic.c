#include "motor/basic.h"

const uint8_t hall_seq_clw[8] = {0, 5, 3, 1, 6, 4, 2, 0};
const uint8_t hall_seq_ccw[8] = {0, 3, 6, 2, 5, 1, 4, 0};

MotorParameter motor_h = {
    .const_h = {
        .adc_u_id = 0,
        .adc_v_id = 1,
        .adc_w_id = 2,
        .Hall_GPIOx         = { GPIOC,       GPIOC,       GPIOC       },
        .Hall_GPIO_Pin_x    = { GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_10 },
        // H: PC0 PC1 PC2 // L: PB13 PB14 PB15
        .htimx          = &htim1,
        .TIM_CHANNEL_x  = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 },
        // 120 deg control L
        .Coil_GPIOx         = { GPIOB,       GPIOB,       GPIOB       },
        .Coil_GPIO_Pin_x    = { GPIO_PIN_5,  GPIO_PIN_4,  GPIO_PIN_10 },
        .ELE_htimx = &htim3,
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
    .pwm_it_angle_acc = 0.0f,
};

Result motor_hall_to_angle(uint8_t hall, float *angle)
{
    switch(hall)
    {
        case 5:
        {
            *angle = 0.0f * DEG_TO_RAD;
            break;
        }
        case 4:
        {
            *angle = 60.0f * DEG_TO_RAD;
            break;
        }
        case 6:
        {
            *angle = 120.0f * DEG_TO_RAD;
            break;
        }
        case 2:
        {
            *angle = 180.0f * DEG_TO_RAD;
            break;
        }
        case 3:
        {
            *angle = 240.0f * DEG_TO_RAD;
            break;
        }
        case 1:
        {
            *angle = 300.0f * DEG_TO_RAD;
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
    return RESULT_OK(NULL);
}

float clampf(float val, float min, float max)
{
    if (val > max) return max;
    if (val < min) return min;
    return val;
}
