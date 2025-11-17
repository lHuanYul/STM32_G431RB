#include "motor/ctrl_deg.h"

// Commutation right_SEQUENCE for degree control
#define HIGH_PASS   1
#define NONE_PASS   0
#define LOW_PASS   -1
static const int8_t seq_map_120[6][3] = {
  { HIGH_PASS, LOW_PASS,  NONE_PASS },
  { HIGH_PASS, NONE_PASS, LOW_PASS  },
  { NONE_PASS, HIGH_PASS, LOW_PASS  },
  { LOW_PASS,  HIGH_PASS, NONE_PASS },
  { LOW_PASS,  NONE_PASS, HIGH_PASS },
  { NONE_PASS, LOW_PASS,  HIGH_PASS }
};
static const int8_t seq_map_180[6][3] = {
    { HIGH_PASS, LOW_PASS,  LOW_PASS  }, // 6
    { HIGH_PASS, HIGH_PASS, LOW_PASS  }, // 2
    { LOW_PASS,  HIGH_PASS, LOW_PASS  }, // 3
    { LOW_PASS,  HIGH_PASS, HIGH_PASS }, // 1
    { LOW_PASS,  LOW_PASS,  HIGH_PASS }, // 5
    { HIGH_PASS, LOW_PASS,  HIGH_PASS }  // 4
};
static const uint8_t seq_index_ccw[] = {0xFF, 5, 3, 4, 1, 0, 2, 0xFF};

inline void deg_ctrl_load(const MotorParameter *motor)
{
    uint8_t idx = seq_index_ccw[motor->exti_hall_curt];
    if (motor->pi_spd.Ref < 0)
    {
        switch (motor->mode)
        {
            case MOTOR_CTRL_120:
            {
                idx = (idx + 3) % 6;
                break;
            }
            case MOTOR_CTRL_180:
            {
                idx = (idx + 2) % 6;
                break;
            }
            default: return;
        }
    }
    uint8_t i;
    uint32_t compare = (uint32_t)((float32_t)TIM1_ARR * motor->pwm_duty_deg);
    for (i = 0; i < 3; i++)
    {
        int8_t state;
        switch (motor->mode)
        {
            case MOTOR_CTRL_120:
            {
                state = seq_map_120[idx][i];
                break;
            }
            case MOTOR_CTRL_180:
            {
                state = seq_map_180[idx][i];
                break;
            }
            default: return;
        }
        switch (state)
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
                __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i], motor->const_h.PWM_htimx->Init.Period);
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
