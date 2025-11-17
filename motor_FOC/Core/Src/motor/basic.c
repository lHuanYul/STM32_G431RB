#include "motor/basic.h"
#include "tim.h"
#include "main/tim.h"

const uint8_t hall_seq_ccw[8] = {0, 5, 3, 1, 6, 4, 2, 0};
const uint8_t hall_seq_clw[8] = {0, 3, 6, 2, 5, 1, 4, 0};
MotorParameter motor_h = {
    .const_h = {
        .Hall_GPIOx         = { GPIOC,       GPIOC,       GPIOC       },
        .Hall_GPIO_Pin_x    = { GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_10 },
        // H: PC0 PC1 PC2 / L: PB13 PB14 PB15
        // Yellow Green Blue
        .PWM_htimx          = &htim1,
        .PWM_TIM_CHANNEL_x  = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 },
        .PWM_tim_clk        = &tim_clk_APB2,
        .PWM_MID_TIM_CH_x   = TIM_CHANNEL_4,
        // .Coil_GPIOx         = { GPIOB,       GPIOB,       GPIOB       },
        // .Coil_GPIO_Pin_x    = { GPIO_PIN_5,  GPIO_PIN_4,  GPIO_PIN_10 },
        .IT20k_htimx        = &htim3,
        .IT20k_tim_clk      = &tim_clk_APB1,
        .SPD_htimx          = &htim2,
        .SPD_tim_clk        = &tim_clk_APB1,
        // 42BLF01
        .rated_current      = MOTOR_42BLF01_RATED_CURRENT,
        .peak_current       = MOTOR_42BLF01_PEAK_CURRENT,
    },
    // Yellow
    .adc_a = &adc_0,
    // Green
    .adc_b = &adc_1,
    // Blue
    .adc_c = &adc_2,
    .pi_spd = {
        .Kp = 0.000025f,
        .Ki = 0.002f,
        .Umax = 0.1f,
        .Umin = -0.1f,
        .w1 = 1.0f,
    },
    .pi_Id = {
        .Ref = 0.0f,
        .Kp = 0.2f,
        .Ki = 0.001f,
        .Umax = 0.01f,
        .Umin = -0.01f,
        .w1 = 1.0f,
    },
    .pi_Iq = {
        .Ref = 0.5f,
        .Kp = 0.3f,
        .Ki = 0.001f,
        // .Umax = 0.75f, In motor_init
        // .Umin = -0.75f, In motor_init
        .w1 = 1.0f,
    },
    .pwm_duty_deg = 1.0f,
};

inline void motor_init(MotorParameter *motor)
{
    motor->pi_Iq.Umax =  motor->const_h.rated_current;
    motor->pi_Iq.Umin = -motor->const_h.rated_current;
}

void motor_set_speed(MotorParameter *motor, float32_t speed)
{
    // speed = var_clampf(speed, -100.0f, 100.0f);
    motor->pi_spd.Ref = speed;
}

void motor_switch_ctrl(MotorParameter *motor, MotorCtrlMode ctrl)
{
    switch (ctrl)
    {
        case MOTOR_CTRL_120:
        case MOTOR_CTRL_180:
        {
            break;
        }
        case MOTOR_CTRL_FOC:
        {
            HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
            HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
            HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
            HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
            HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
            HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
            break;
        }
        default: return;
    }
    motor->mode = ctrl;
}
