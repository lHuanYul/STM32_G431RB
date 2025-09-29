#pragma once

#include "main/config.h"
#include "motor/pi.h"
#include "motor/clarke.h"
#include "motor/park.h"
#include "motor/svgendq.h"

extern const uint8_t hall_seq_clw[8];
extern const uint8_t hall_seq_ccw[8];

// #define MS_HIGH_PASS   1
// #define MS_NONE_PASS   0
// #define MS_LOW_PASS    -1
// extern const int8_t motor_sequence[6][3];

typedef struct MotorConst {
    uint8_t adc_u_id;
    uint8_t adc_v_id;
    uint8_t adc_w_id;
    GPIO_TypeDef*       Hall_GPIOx[3];
    uint16_t            Hall_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  htimx[3];
    uint32_t            TIM_CHANNEL_x[3];
    GPIO_TypeDef*       Coil_GPIOx[3];
    uint16_t            Coil_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  ELE_htimx;
} MotorConst;

typedef struct MotorParameter {
    const MotorConst const_h;
    float          rpm_fbk_hall;
    float          rpm_fbk_htim;
    volatile PI_CONTROLLER  pi_speed;
    float          pi_speed_cmd;
    PI_CONTROLLER  pi_Iq;
    PI_CONTROLLER  pi_Id;
    uint8_t    exti_hall_last;         // GPIO trigger
    volatile uint8_t    exti_hall_curt;         // GPIO trigger
    volatile uint16_t   exti_hall_curt_d;       // GPIO trigger
    volatile uint16_t   exti_hall_cnt;          // GPIO trigger
    volatile float      hall_angle_acc;         // 霍爾累積角度基準 每次霍爾相位切換時 馬達轉+60角度
    uint8_t    pwm_hall_last;          // PWM last hall record
    volatile uint16_t   pwm_hall_acc;           // PWM hall record total
    uint16_t   spin_stop_acc;
    volatile float  pwm_per_it_angle_itpl;  // PWM 中斷應補角度 (Angle Interpolation)
    volatile float  pwm_it_angle_acc;
    uint16_t   pwm_count;
    uint16_t   adc_u;
    uint16_t   adc_v;
    uint16_t   adc_w;
    CLARKE     clarke;
    PARK       park;
    IPARK      ipark;
    SVGENDQ    svgendq;
    float      electric_theta_rad;
    float      electric_theta_deg;
    float      svpwm_Vref;
    float      pwm_duty_u;
    float      pwm_duty_v;
    float      pwm_duty_w;
    bool                reverse;
} MotorParameter;

extern MotorParameter motor_0;

Result hall_to_angle(uint8_t hall, volatile uint16_t *angle);
void step_commutate_120(const MotorParameter *motor);
