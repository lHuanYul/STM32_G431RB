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
    .rpm_iq_rated = {
        
    },
    .rpm_iq_peak = {
    },
    .pi_spd = {
        .ref = 0.0f,
        .Kp = 0.000025f,
        .Ki = 0.002f,
        .max = 0.1f,
        .min = -0.1f,
        .sat = 1.0f,
    },
    .pi_Iq = {
        .Kp = 0.3f,
        .Ki = 0.001f,
        // .max = 0.75f, In motor_init
        // .min = -0.75f, In motor_init
        .sat = 1.0f,
    },
    .pi_Id = {
        .Kp = 0.2f,
        .Ki = 0.001f,
        .max = 0.01f,
        .min = -0.01f,
        .sat = 1.0f,
    },
    .pwm_duty_deg = 1.0f,
};

inline void motor_init(MotorParameter *motor)
{
    motor->pi_Iq.max =  motor->const_h.rated_current;
    motor->pi_Iq.min = -motor->const_h.rated_current;
}

void motor_set_speed(MotorParameter *motor, float32_t speed)
{
    if (speed > 0)
    {
        motor->rpm_ref.reverse = 0;
        motor->rpm_ref.value = speed;
    }
    else if (speed < 0)
    {
        motor->rpm_ref.reverse = 1;
        motor->rpm_ref.value = -speed;
    }
    else motor->rpm_ref.value = 0;
}

inline void motor_set_rotate_mode(MotorParameter *motor, MotorModeRot mode)
{
    motor->mode_rot = mode;
}

void motor_switch_ctrl(MotorParameter *motor, MotorModeCtrl ctrl)
{
    switch (ctrl)
    {
        case MOTOR_CTRL_120:
        case MOTOR_CTRL_180:
        {
            break;
        }
        case MOTOR_CTRL_FOC_PEAK:
        case MOTOR_CTRL_FOC_RATED:
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
    motor->mode_ctrl = ctrl;
}
