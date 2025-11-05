#include "motor/basic.h"
#include "tim.h"
#include "main/tim.h"

const uint8_t hall_seq_clw[8] = {0, 5, 3, 1, 6, 4, 2, 0};
const uint8_t hall_seq_ccw[8] = {0, 3, 6, 2, 5, 1, 4, 0};

MotorParameter motor_h = {
    .const_h = {
        // HALL PIN
        .Hall_GPIOx         = { GPIOC,       GPIOC,       GPIOC       },
        .Hall_GPIO_Pin_x    = { GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_10 },
        // PWM TIM
        // H: PC0 PC1 PC2 // L: PB13 PB14 PB15
        .PWM_htimx          = &htim1,
        .PWM_TIM_CHANNEL_x  = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 },
        // 120 deg control L
        .Coil_GPIOx         = { GPIOB,       GPIOB,       GPIOB       },
        .Coil_GPIO_Pin_x    = { GPIO_PIN_5,  GPIO_PIN_4,  GPIO_PIN_10 },
        // FOC TIM
        .FOC_htimx          = &htim1,
        .FOC_tim_clk        = &tim_clk_APB2,
        // ELE
        .SPD_htimx          = &htim3,
        .SPD_tim_clk        = &tim_clk_APB1,
    },
    .adc_u = {
        .const_h = {
            .id = 0,
            .sensitive = 0.097f,
        },
        .current_trs = 1.0f,
    },
    .adc_v = {
        .const_h = {
            .id = 1,
            .sensitive = 0.1f,
        },
        .current_trs = 1.0f,
    },
    .adc_w = {
        .const_h = {
            .id = 2,
            .sensitive = 0.1f,
        },
        .current_trs = 1.0f,
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
    .foc_angle_acc = 0.0f,
};

inline float32_t clampf(float32_t val, float32_t min, float32_t max)
{
    if (val > max) return max;
    if (val < min) return min;
    return val;
}

inline float32_t wrap_0_2pi(float32_t x)
{
    int32_t n = (int32_t)(x / PI_MUL_2);
    x -= (float32_t)n * PI_MUL_2;
    if (x < 0) x += PI_MUL_2;
    return x;
}

inline float32_t wrap_m1_1pi(float32_t x)
{
    int32_t n = (int32_t)(x / PI_MUL_2);
    x -= (float32_t)n * PI_MUL_2;
    if      (x < -PI) x += PI_MUL_2;
    else if (x >= PI) x -= PI_MUL_2;
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

inline void motor_set_speed(MotorParameter *motor, float32_t speed)
{
    motor->pi_speed.Ref = speed;
}

inline void motor_set_direction(MotorParameter *motor, bool drct)
{
    motor->reverse = drct;
}
