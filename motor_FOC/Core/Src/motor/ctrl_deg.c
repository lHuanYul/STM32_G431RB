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

uint32_t dbg_check;

void deg_ctrl_120_load(MotorParameter *motor)
{
    uint8_t idx;
    if (motor->mode_rotate == MOTOR_ROT_LOCK_CHK)
        idx = index_180[motor->exti_hall_curent];
    else
    {
        idx = index_120_ccw[motor->exti_hall_curent];
        if (motor->rpm_reference.reverse) idx = (idx + 3) % 6;
    }
    float32_t *duty[3] = {&motor->pwm_duty_u, &motor->pwm_duty_v, &motor->pwm_duty_w};
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        switch (seq_map_120[idx][i])
        {
            case HIGH_PASS:
            {
                *duty[i] = motor->pwm_duty_deg;
                break;
            }
            case LOW_PASS:
            default:
            {
                *duty[i] = 0;
                break;
            }
        }
    }
    motor_pwm_load(motor);
}

void deg_ctrl_180_load(MotorParameter *motor)
{
    uint8_t idx, i;
    float32_t *duty[3] = {&motor->pwm_duty_u, &motor->pwm_duty_v, &motor->pwm_duty_w};
    switch (motor->mode_rotate)
    {
        case MOTOR_ROT_COAST:
        {
            for (i = 0; i < 3; i++)
            {
                HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            }
            return;
        }
        case MOTOR_ROT_NORMAL:
        case MOTOR_ROT_LOCK:
        {
            idx = (!motor->rpm_reference.reverse) ?
                  index_180[motor->exti_hall_curent] + 2  // + 1 / 2
                : index_180[motor->exti_hall_curent] + 4; // + 5 / 4
            idx %= 6;
            break;
        }
        case MOTOR_ROT_LOCK_CHK:
        {
            idx = index_180[motor->exti_hall_curent];
            break;
        }
    }
    for (i = 0; i < 3; i++)
    {
        HAL_TIM_PWM_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
        HAL_TIMEx_PWMN_Start(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
        switch (seq_map_180[idx][i])
        {
            case HIGH_PASS:
            {
                // *duty[i] = 0;
                *duty[i] = motor->pwm_duty_deg;
                break;
            }
            case LOW_PASS:
            {
                *duty[i] = 0;
                break;
            }
        }
    }
    motor_pwm_load(motor);
}
