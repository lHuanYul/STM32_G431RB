#pragma once

#include "main/config.h"
#include "motor/pi.h"
#include "motor/clarke.h"
#include "motor/park.h"
#include "motor/svgendq.h"
#include "main/variable_cal.h"
#include "arm_math.h"
#include "cordic.h"

extern const uint8_t hall_seq_clw[8];
extern const uint8_t hall_seq_ccw[8];

typedef struct MotorConst
{
    uint8_t             adc_u_id;
    uint8_t             adc_v_id;
    uint8_t             adc_w_id;
    GPIO_TypeDef*       Hall_GPIOx[3];
    uint16_t            Hall_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  htimx;
    uint32_t            TIM_CHANNEL_x[3];
    GPIO_TypeDef*       Coil_GPIOx[3];
    uint16_t            Coil_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  ELE_htimx;
} MotorConst;

typedef enum MotorCtrlMode
{
    MOTOR_CTRL_120,
    MOTOR_CTRL_FOC,
} MotorCtrlMode;

typedef struct MotorParameter
{
    const MotorConst    const_h;
    MotorCtrlMode       mode;
    float32_t           rpm_fbk_hall;
    float32_t           rpm_fbk_htim;
    uint8_t             exti_hall_last;         // GPIO trigger
    volatile uint8_t    exti_hall_curt;         // GPIO trigger
    volatile float32_t  exti_hall_angal;
    volatile uint16_t   exti_hall_cnt;          // GPIO trigger
    volatile float32_t  hall_angle_acc;         // 霍爾累積角度基準 每次霍爾相位切換時 馬達轉+60角度
    uint8_t             pwm_hall_last;          // PWM last hall record
    volatile uint16_t   pwm_hall_acc;           // PWM hall record total
    volatile float32_t  pwm_per_it_angle_itpl;  // PWM 中斷應補角度 (Angle Interpolation)
    volatile float32_t  pwm_it_angle_acc;
    uint16_t            pwm_count;
    uint16_t            spin_stop_acc;
    float32_t           adc_u;
    float32_t           adc_v;
    float32_t           adc_w;
    volatile PI_CTRL    pi_speed;
    float32_t           pi_speed_cmd;
    PI_CTRL             pi_Iq;
    PI_CTRL             pi_Id;
    CLARKE              clarke;
    PARK                park;
    IPARK               ipark;
    SVGENDQ             svgendq;
    float32_t           elec_theta_rad;
    float32_t           elec_theta_deg;
    float32_t           svpwm_Vref;
    float32_t           pwm_duty_u;
    float32_t           pwm_duty_v;
    float32_t           pwm_duty_w;
    bool                reverse;
} MotorParameter;
extern MotorParameter motor_h;

Result motor_hall_to_angle(uint8_t hall, volatile float32_t *angle);
float32_t clampf(float32_t val, float32_t min, float32_t max);
float32_t wrap_0_2pi(float32_t x);
float32_t wrap_m1_1pi(float32_t x);
float32_t fast_fabsf(float32_t x);
