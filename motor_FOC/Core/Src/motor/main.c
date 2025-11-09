#include "motor/main.h"
#include "tim.h"
#include "motor/ctrl_deg.h"
#include "motor/ctrl_foc.h"
#include "analog/adc1/main.h"
#include "motor/trigonometric.h"

static void hall_update(MotorParameter *motor)
{
    uint8_t hall_last = motor->exti_hall_curt;
    uint8_t hall_current =
          ((motor->const_h.Hall_GPIOx[0]->IDR & motor->const_h.Hall_GPIO_Pin_x[0]) ? 4U : 0U)
        | ((motor->const_h.Hall_GPIOx[1]->IDR & motor->const_h.Hall_GPIO_Pin_x[1]) ? 2U : 0U)
        | ((motor->const_h.Hall_GPIOx[2]->IDR & motor->const_h.Hall_GPIO_Pin_x[2]) ? 1U : 0U);
    motor->exti_hall_last = hall_last;
    motor->exti_hall_curt = hall_current;
    if (hall_current == 0 || hall_current == 7) 
    {
        uint8_t i;
        for (i = 0; i < 3; i++)
        {
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[i]);
            HAL_GPIO_WritePin(motor->const_h.Coil_GPIOx[i], motor->const_h.Coil_GPIO_Pin_x[i],  GPIO_PIN_RESET);
        }
        return;
    }
}

uint32_t htim_cnt;
void motor_hall_exti(MotorParameter *motor)
{
    hall_update(motor);
    // 0: ccw / 1: clw
    bool reverse;
    bool expected = 0;
    if (
        motor->exti_hall_curt == hall_seq_ccw[motor->exti_hall_last]
        // || motor->exti_hall_last == 0
    ) {
        expected = 1;
        reverse = 0;
    }
    else if (motor->exti_hall_curt == hall_seq_clw[motor->exti_hall_last])
    {
        expected = 1;
        reverse = 1;
    }
    // rpm
    motor->exti_hall_cnt++;
    if (motor->exti_hall_cnt >= 6)
    {
        motor->exti_hall_cnt = 0;
        htim_cnt = __HAL_TIM_GET_COUNTER(motor->const_h.SPD_htimx);
        __HAL_TIM_SET_COUNTER(motor->const_h.SPD_htimx, 0);
        if (htim_cnt == 0)
        {
            motor->rpm_fbk = 0.0f;
        }
        else
        {
            motor->rpm_fbk = motor->tfm_rpm_fbk / (float32_t)htim_cnt;
            motor->tim_angle_itpl = motor->tfm_tim_it_angle_itpl / (float32_t)htim_cnt;
            if (reverse)
            {
                motor->rpm_fbk *= -1;
                motor->tim_angle_itpl *= -1;
            }
        }
    }
    
    #ifndef MOTOR_FOC_SPIN_DEBUG
    switch (motor->mode)
    {
        case MOTOR_CTRL_120:
        {
            motor_deg_rotate(motor);
            break;
        }
        case MOTOR_CTRL_FOC:
        {
            // 確認旋轉正確
            if (expected && (motor->reverse == reverse))
            {
                motor->tim_angle_acc = 0.0f;
            }
            motor_angle_trsf(motor);
            break;
        }
    }
    #else
    motor->tim_angle_itpl = motor->tfm_tim_it_angle_itpl / htim_cnt;
    motor_angle_trsf(motor);
    motor_deg_rotate(motor);
    #endif
}

static inline void stop_check(MotorParameter *motor)
{
    // 停轉判斷
    // 現在與上一個霍爾的總和與之前的總和相同，視為馬達靜止不動
    uint8_t current = motor->exti_hall_curt;
    uint16_t total = motor->chk_hall_last * 10 + current;
    if(total == motor->chk_hall_total)
    {
        motor->stop_spin_acc++;
        if (motor->stop_spin_acc >= MOTOR_STOP_TRI)
        {
            motor->stop_spin_acc = 0;
            motor->rpm_fbk = 0;         // 歸零速度實際值
            __HAL_TIM_SET_COUNTER(motor->const_h.SPD_htimx, 0);
            motor->pi_speed.i1 = 0;     // 重置i控制舊值
            motor_foc_rot_stop(motor);
        }
    }
    else motor->stop_spin_acc = 0;
    motor->chk_hall_last = current;
    motor->chk_hall_total = total;
}

static inline void pi_speed(MotorParameter *motor)
{
    // 計算 速度PI (每100個PWM中斷)
    motor->pi_speed.Fbk = motor->rpm_fbk;
    PI_run(&motor->pi_speed);
    motor->pi_speed_cmd = clampf((motor->pi_speed_cmd + motor->pi_speed.Out), 0.15f, 0.2f);
}

void motor_pwm_pulse(MotorParameter *motor)
{
    motor->dbg_tim_it_cnt++;
    if (motor->dbg_tim_it_cnt >= 100)
    {
        motor->dbg_tim_it_cnt = 0;
        stop_check(motor);
        pi_speed(motor);
    }
    RESULT_CHECK_RET_VOID(adc_renew(&motor->adc_u));
    RESULT_CHECK_RET_VOID(adc_renew(&motor->adc_v));
    RESULT_CHECK_RET_VOID(adc_renew(&motor->adc_w));
    #ifndef MOTOR_FOC_SPIN_DEBUG
    switch (motor->mode)
    {
        case MOTOR_CTRL_120:
        {
            break;
        }
        case MOTOR_CTRL_FOC:
        {
            motor_foc_pwm_pulse(motor);
            break;
        }
    }
    #else
    motor_foc_pwm_pulse(motor);
    #endif
}

static void motor_init(MotorParameter *motor)
{
    const float32_t FOC_tim_f =
        (float32_t)*motor->const_h.IT20k_tim_clk /
        (float32_t)(motor->const_h.IT20k_htimx->Init.Prescaler + 1U);
    // ELE_tim_f：霍爾計時器的實際計數頻率 (Hz)
    // = ELE_timer_clock / (PSC + 1)
    const float32_t ELE_tim_f =
        (float32_t)*motor->const_h.SPD_tim_clk /
        (float32_t)(motor->const_h.SPD_htimx->Init.Prescaler + 1U);
    // TIM_tim_t：PWM 控制定時器每個計數週期的時間 (秒/計數)
    // = (PSC + 1) / TIM_timer_clock
    const float32_t FOC_tim_t =
        (float32_t)(motor->const_h.IT20k_htimx->Init.Prescaler + 1U) /
        (float32_t)*motor->const_h.IT20k_tim_clk;
    // ELE_tim_t：霍爾計時器每個計數週期的時間 (秒/計數)
    // = (PSC + 1) / ELE_timer_clock
    const float32_t ELE_tim_t =
        (float32_t)(motor->const_h.SPD_htimx->Init.Prescaler + 1U) /
        (float32_t)*motor->const_h.SPD_tim_clk;
    motor->dbg_tim_it_freq = FOC_tim_f / motor->const_h.IT20k_htimx->Init.Period;
    motor->tfm_rpm_fbk =
        ELE_tim_f / (6.0f * (float32_t)MOTOR_42BLF01_POLE / 2.0f * MOTOR_42BLF01_GEAR) * 60.0f;
    motor->tfm_tim_it_angle_itpl =
        FOC_tim_t / ELE_tim_t * (float32_t)(motor->const_h.IT20k_htimx->Init.Period) * PI_DIV_3;

    HAL_TIM_Base_Start_IT(motor->const_h.PWM_htimx);
    HAL_TIM_Base_Start(motor->const_h.SPD_htimx);
    
    HAL_TIM_Base_Start(&htim3);
}

inline void motor_set_speed(MotorParameter *motor, float32_t speed)
{
    motor->pi_speed.Ref = speed;
}

// 0: ccw / 1: clw
inline void motor_set_direction(MotorParameter *motor, bool reverse)
{
    motor->reverse = reverse;
}

void motor_switch_ctrl(MotorParameter *motor, MotorCtrlMode ctrl)
{
    switch (ctrl)
    {
        case MOTOR_CTRL_120:
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
        default:
        {
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
            HAL_TIM_PWM_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[0]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[1]);
            HAL_TIMEx_PWMN_Stop(motor->const_h.PWM_htimx, motor->const_h.PWM_TIM_CHANNEL_x[2]);
            break;
        }
    }
    motor->mode = ctrl;
}

void StartMotorTask(void *argument)
{
    while(HAL_GetTick() < 1000)
    {
        RESULT_CHECK_HANDLE(adc_renew(&motor_h.adc_u));
        RESULT_CHECK_HANDLE(adc_renew(&motor_h.adc_v));
        RESULT_CHECK_HANDLE(adc_renew(&motor_h.adc_w));
        osDelay(1);
    }
    adc_init(&motor_h.adc_u);
    adc_init(&motor_h.adc_v);
    adc_init(&motor_h.adc_w);
    motor_init(&motor_h);
    motor_h.pi_speed.Ref = 100.0f;
    motor_set_direction(&motor_h, 1);

    motor_switch_ctrl(&motor_h, MOTOR_CTRL_120);
    motor_hall_exti(&motor_h);
    osDelay(3000);

    motor_switch_ctrl(&motor_h, MOTOR_CTRL_FOC);
    motor_hall_exti(&motor_h);
    StopTask();
}
