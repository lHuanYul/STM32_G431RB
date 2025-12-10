#include "motor/basic.h"
#include "tim.h"
#include "main/tim.h"

const uint8_t hall_seq_ccw[8] = {UINT8_MAX, 5, 3, 1, 6, 4, 2, UINT8_MAX};
const uint8_t hall_seq_clw[8] = {UINT8_MAX, 3, 6, 2, 5, 1, 4, UINT8_MAX};
MotorParameter motor_h = {
    .const_h = {
        // Yellow Green Blue
        .Hall_GPIOx         = { GPIOC,       GPIOC,       GPIOC       },
        .Hall_GPIO_Pin_x    = { GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12 },
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
    .pi_speed = {
        .reference = 0.0f,
        .Kp = 0.000025f,
        .Ki = 0.002f,
        .max = 0.1f,
        .min = -0.1f,
        .saturation = 1.0f,
    },
    .tfm_duty_Iq = 1.0f,
    .pi_Iq = {
        .Kp = 0.3f,
        .Ki = 0.001f,
        // .max = 0.75f, In motor_init
        // .min = -0.75f, In motor_init
        .saturation = 1.0f,
    },
    .pi_Id = {
        .Kp = 0.2f,
        .Ki = 0.001f,
        .max = 0.01f,
        .min = -0.01f,
        .saturation = 1.0f,
    },
    .hall_start = 4,
    .rpm_save_stop = 10.0f,
};

void motor_init(MotorParameter *motor)
{
    motor->pi_Iq.max =  motor->const_h.rated_current;
    motor->pi_Iq.min = -motor->const_h.rated_current;
}

void motor_set_rpm(MotorParameter *motor, bool reverse, float32_t speed)
{
    if (speed != 0) motor->rpm_user.reverse = reverse;
    motor->rpm_user.value = speed;
}

void motor_set_rotate_mode(MotorParameter *motor, MotorModeRotate mode)
{
    if (
        mode != MOTOR_ROT_COAST &&
        mode != MOTOR_ROT_NORMAL &&
        mode != MOTOR_ROT_LOCK &&
        mode != MOTOR_ROT_LOCK_CHK
    ) return;
    if (mode == MOTOR_ROT_LOCK_CHK) mode = MOTOR_ROT_LOCK;
    motor->mode_rotate = mode;
}

void motor_alive(MotorParameter *motor)
{
    motor->alive_tick = HAL_GetTick();
}

void motor_switch_ctrl(MotorParameter *motor, MotorModeControl ctrl)
{
    switch (ctrl)
    {
        case MOTOR_CTRL_120:
        case MOTOR_CTRL_180:
        case MOTOR_CTRL_FOC_PEAK:
        case MOTOR_CTRL_FOC_RATED:
        {
            // uint8_t i;
            // for (i = 0; i < 3; i++)
            // {
            //     HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            //     HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            // }
            break;
        }
    }
    motor->mode_control = ctrl;
}

void motor_pwm_load(MotorParameter *motor)
{
    VAR_CLAMPF(motor->pwm_duty_u, 0.0f, 1.0f);
    VAR_CLAMPF(motor->pwm_duty_v, 0.0f, 1.0f);
    VAR_CLAMPF(motor->pwm_duty_w, 0.0f, 1.0f);
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0],
        (uint32_t)(motor->tfm_pwm_period * motor->pwm_duty_u));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1],
        (uint32_t)(motor->tfm_pwm_period * motor->pwm_duty_v));
    __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2],
        (uint32_t)(motor->tfm_pwm_period * motor->pwm_duty_w));
}
