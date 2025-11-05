#include "motor/ctrl_120.h"

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

// 120 deg cntrol
void motor_120_hall_update(const MotorParameter *motor)
{
    uint8_t idx = motor_seq_index[motor->exti_hall_curt];
    if (motor->reverse) idx = (idx + 3) % 6; // CCW
    // idx = 5;
    uint8_t i;
    uint32_t compare = (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_120);
    for (i = 0; i < 3; i++)
    {
        switch (motor_seq_map[idx][i])
        {
            case HIGH_PASS:
            {
                __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i], compare);
                HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                // HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
                break;
            }
            case LOW_PASS:
            {
                HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i], TIM1_ARR);
                HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                // HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_SET);
                break;
            }
            default:
            {
                HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                // HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
                break;
            }
        }
    }
}
