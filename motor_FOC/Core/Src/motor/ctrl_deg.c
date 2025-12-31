#include "motor/ctrl_deg.h"

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
static const uint8_t index_120_ccw[] = {7, 4, 2, 3, 0, 5, 1, 7};
static const int8_t seq_map_180[][3] = {
    { HIGH_PASS, LOW_PASS,  HIGH_PASS }, // 0-4
    { HIGH_PASS, LOW_PASS,  LOW_PASS  }, // 1-6
    { HIGH_PASS, HIGH_PASS, LOW_PASS  }, // 2-2
    { LOW_PASS,  HIGH_PASS, LOW_PASS  }, // 3-3
    { LOW_PASS,  HIGH_PASS, HIGH_PASS }, // 4-1
    { LOW_PASS,  LOW_PASS,  HIGH_PASS }, // 5-5
    { HIGH_PASS, HIGH_PASS, HIGH_PASS }, // 6
    { LOW_PASS,  LOW_PASS,  LOW_PASS  }, // 7
};
static const uint8_t index_180_lock[] = {7, 4, 2, 3, 0, 5, 1, 7};
static const uint8_t index_180_ccw[]  = {7, 0, 4, 5, 2, 1, 3, 7};
static const uint8_t index_180_cw[]   = {7, 2, 0, 1, 4, 3, 5, 7};

void deg_ctrl_120_load(MotorParameter *motor)
{
    if (motor->hall_current == UINT8_MAX) return;
    uint8_t idx;
    if (motor->mode_rot_ref == MOTOR_ROT_LOCK_CHK)
        idx = index_180_lock[motor->hall_current];
    else
    {
        idx = index_120_ccw[motor->hall_current];
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
                TIM_CH_ENABLE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                *duty[i] = motor->pwm_duty_deg;
                break;
            }
            case LOW_PASS:
            {
                TIM_CH_ENABLE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                *duty[i] = 0.0f;
                break;
            }
            default:
            {
                TIM_CH_DISABLE(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
                *duty[i] = 0.5f;
                break;
            }
        }
    }
    motor_pwm_load(motor);
}

void deg_ctrl_180_load(MotorParameter *motor)
{
    if (motor->hall_current == UINT8_MAX) return;
    uint8_t i;
    float32_t *duty[3] = {
        &motor->pwm_duty_u,
        &motor->pwm_duty_v,
        &motor->pwm_duty_w
    };
    int8_t seq[3] = {0};
    switch (motor->mode_rot_ref)
    {
        case MOTOR_ROT_COAST:
        {
            motor->pwm_duty_deg = 1.0f;
            for (i = 0; i < 3; i++) seq[i] = seq_map_180[6][i];
            break;
        }
        case MOTOR_ROT_BREAK:
        case MOTOR_ROT_LOCK:
        {
            for (i = 0; i < 3; i++) seq[i] = seq_map_180[7][i];
            break;
        }
        case MOTOR_ROT_NORMAL:
        {
            for (i = 0; i < 3; i++)
            {
                if (!motor->rpm_reference.reverse)
                    seq[i] = seq_map_180[index_180_ccw[motor->hall_current]][i];
                else
                    seq[i] = seq_map_180[ index_180_cw[motor->hall_current]][i];
            }
            break;
        }
        case MOTOR_ROT_LOCK_CHK:
        {
            motor->pwm_duty_deg = 0.2f;
            for (i = 0; i < 3; i++)
                seq[i] = seq_map_180[index_180_lock[motor->hall_current]][i];
            break;
        }
    }
    for (i = 0; i < 3; i++)
    {
        if (seq[i] == HIGH_PASS)
            *duty[i] = motor->pwm_duty_deg;
        else
            *duty[i] = 0;
    }
    motor_pwm_load(motor);
}
