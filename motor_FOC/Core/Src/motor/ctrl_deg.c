#include "motor/ctrl_deg.h"

// Commutation right_SEQUENCE for degree control
#define HIGH_PASS   1
#define NONE_PASS   0
#define LOW_PASS   -1
// static const int8_t seq_map_120[6][3] = {
//     { HIGH_PASS, LOW_PASS,  NONE_PASS },
//     { HIGH_PASS, NONE_PASS, LOW_PASS  },
//     { NONE_PASS, HIGH_PASS, LOW_PASS  },
//     { LOW_PASS,  HIGH_PASS, NONE_PASS },
//     { LOW_PASS,  NONE_PASS, HIGH_PASS },
//     { NONE_PASS, LOW_PASS,  HIGH_PASS },
// };
// static const uint8_t index_120_ccw[] = {0xFF, 5, 3, 4, 1, 0, 2, 0xFF};
static const int8_t seq_map_120[6][3] = {
    { HIGH_PASS, NONE_PASS, LOW_PASS  },
    { NONE_PASS, HIGH_PASS, LOW_PASS  },
    { LOW_PASS,  HIGH_PASS, NONE_PASS },
    { LOW_PASS,  NONE_PASS, HIGH_PASS },
    { NONE_PASS, LOW_PASS,  HIGH_PASS },
    { HIGH_PASS, LOW_PASS,  NONE_PASS },
};
static const uint8_t index_120_ccw[] = {0xFF, 4, 2, 3, 0, 5, 1, 0xFF};
static const int8_t seq_map_180[6][3] = {
    { HIGH_PASS, LOW_PASS,  HIGH_PASS }, // 0-4
    { HIGH_PASS, LOW_PASS,  LOW_PASS  }, // 1-6
    { HIGH_PASS, HIGH_PASS, LOW_PASS  }, // 2-2
    { LOW_PASS,  HIGH_PASS, LOW_PASS  }, // 3-3
    { LOW_PASS,  HIGH_PASS, HIGH_PASS }, // 4-1
    { LOW_PASS,  LOW_PASS,  HIGH_PASS }, // 5-5
};
static const uint8_t index_180[] = {0xFF, 4, 2, 3, 0, 5, 1, 0xFF};

inline void deg_ctrl_load(MotorParameter *motor)
{
    uint8_t idx;
    switch (motor->mode_control)
    {
        case MOTOR_CTRL_120:
        {
            if (motor->mode_rotate == MOTOR_ROT_LOCK)
                idx = index_180[motor->exti_hall_curent];
            else
            {
                idx = index_120_ccw[motor->exti_hall_curent];
                if (motor->rpm_reference.reverse) idx = (idx + 3) % 6;
            }
            break;
        }
        case MOTOR_CTRL_180:
        {
            if (motor->mode_rotate == MOTOR_ROT_LOCK)
                idx = index_180[motor->exti_hall_curent];
            else
            {
                idx = (!motor->rpm_reference.reverse) ?
                    index_180[motor->exti_hall_curent] + 2  // + 1 / 2
                    : index_180[motor->exti_hall_curent] + 4; // + 5 / 4
                idx %= 6;
            }
            break;
        }
        default: return;
    }
    uint8_t i;
    uint32_t compare =
        (uint32_t)((float32_t)motor->const_h.PWM_htimx->Init.Period * motor->pwm_duty_deg);
    for (i = 0; i < 3; i++)
    {
        int8_t state;
        switch (motor->mode_control)
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
                HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i],
                    compare);
                HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                // HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
                break;
            }
            case LOW_PASS:
            {
                HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                __HAL_TIM_SET_COMPARE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i],
                    motor->const_h.PWM_htimx->Init.Period);
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
