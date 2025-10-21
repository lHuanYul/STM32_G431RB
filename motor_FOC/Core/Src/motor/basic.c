#include "motor/basic.h"

const uint8_t hall_seq_clw[8] = {0, 5, 3, 1, 6, 4, 2, 0};
const uint8_t hall_seq_ccw[8] = {0, 3, 6, 2, 5, 1, 4, 0};

MotorParameter motor_h = {
    .const_h = {
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
    .adc_u = &adc_u,
    .adc_v = &adc_v,
    .adc_w = &adc_w,
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

inline Result motor_hall_to_angle(uint8_t hall, volatile float32_t *angle)
{
    switch(hall)
    {
        case 6:
        {
            *angle = 0.0f * DEG_TO_RAD;
            break;
        }
        case 2:
        {
            *angle = 60.0f * DEG_TO_RAD;
            break;
        }
        case 3:
        {
            *angle = 120.0f * DEG_TO_RAD;
            break;
        }
        case 1:
        {
            *angle = 180.0f * DEG_TO_RAD;
            break;
        }
        case 5:
        {
            *angle = 240.0f * DEG_TO_RAD;
            break;
        }
        case 4:
        {
            *angle = 300.0f * DEG_TO_RAD;
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
    return RESULT_OK(NULL);
}

inline float32_t clampf(float32_t val, float32_t min, float32_t max)
{
    if (val > max) return max;
    if (val < min) return min;
    return val;
}

inline float32_t wrap_0_2pi(float32_t x)
{
    int32_t n = (int32_t)(x / MUL_2_PI);
    x -= (float32_t)n * MUL_2_PI;
    if (x < 0) x += MUL_2_PI;
    return x;
}

inline float32_t wrap_m1_1pi(float32_t x)
{
    int32_t n = (int32_t)(x / MUL_2_PI);
    x -= (float32_t)n * MUL_2_PI;
    if      (x < -PI) x += MUL_2_PI;
    else if (x >= PI) x -= MUL_2_PI;
    return x;
}

inline float32_t fast_fabsf(float32_t x)
{
    union {
        float32_t f;
        uint32_t u;
    } v = { x };
    v.u &= 0x7FFFFFFF;  // 清除最高位 sign bit
    return v.f;
}
