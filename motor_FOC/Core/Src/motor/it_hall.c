#include "motor/it_hall.h"
#include "main.h"

// Thread - hallExti - exit
static inline Result pos_calculate(MotorParameter *motor)
{
    float htim_cnt = (float)__HAL_TIM_GET_COUNTER(motor->const_h.ELE_htimx);
    __HAL_TIM_SET_COUNTER(motor->const_h.ELE_htimx, 0);
    motor->rpm_fbk_hall = 100000000.0f / htim_cnt;

    // ? check
    // 電氣週期算轉速，分鐘[3G=50,000,000 (計數轉秒)*60(秒轉分鐘)] / 轉速
    // calculate speed every hall instead of  6 times
    // agv gear ratio MOTOR_42BLF01_GEAR
    motor->pi_speed.Fbk = (6000000.0f / (htim_cnt * (MOTOR_42BLF01_POLE / 2))) / 6 / MOTOR_42BLF01_GEAR;
    // 單次PWM中斷時的角度變化 50us*60/(0.1us*CNT)
    motor->pwm_per_it_angle_itpl = 30000.0f / htim_cnt ;

    return RESULT_OK(NULL);
}

Result motor_hall_update(MotorParameter *motor)
{
    motor->exti_hall_cnt++;
    uint8_t hall_last = motor->exti_hall_curt;
    uint8_t hall_current =
          ((motor->const_h.Hall_GPIOx[0]->IDR & motor->const_h.Hall_GPIO_Pin_x[0]) ? 4U : 0U)
        | ((motor->const_h.Hall_GPIOx[1]->IDR & motor->const_h.Hall_GPIO_Pin_x[1]) ? 2U : 0U)
        | ((motor->const_h.Hall_GPIOx[2]->IDR & motor->const_h.Hall_GPIO_Pin_x[2]) ? 1U : 0U);
    motor->exti_hall_last = hall_last;
    motor->exti_hall_curt = hall_current;
    if (hall_current == 0 || hall_current == 7)
    {
        return RESULT_ERROR(RES_ERR_FAIL);
    }
    // 120 deg cntrol
    step_commutate_120(motor);
    
    RESULT_CHECK_RET_RES(hall_to_angle(hall_current, &motor->exti_hall_curt_d));
    uint16_t expected = (!motor->reverse)
        ? hall_seq_clw[hall_last]
        : hall_seq_ccw[hall_last];
    // if (hall_last == 0) // ? CHECK
    // {
    //     hall_last = expected;
    // }
    if (hall_current == expected)
    {
        // rotated
        motor->hall_angle_acc = 0;
        motor->pwm_it_angle_acc = 0;
    }
    return RESULT_OK(NULL);
}

// Thread - hallExti - entrance
Result motor_hall_exti(MotorParameter *motor)
{
    RESULT_CHECK_RET_RES(motor_hall_update(motor));
    RESULT_CHECK_RET_RES(pos_calculate(motor));
    return RESULT_OK(NULL);
}
