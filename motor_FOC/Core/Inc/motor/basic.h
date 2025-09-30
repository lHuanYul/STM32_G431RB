#pragma once

#include "main/config.h"
#include "motor/pi.h"
#include "motor/clarke.h"
#include "motor/park.h"
#include "motor/svgendq.h"

#define PI          3.14159265358979323846f // 180 deg
#define MUL_2_PI    (2.0f * PI)     // 360 deg
#define DIV_PI_2    (PI / 2.0f)     // 90 deg
#define DIV_PI_3    (PI / 3.0f)     // 60 deg
#define DEG_TO_RAD  0.01745329252f  // π/180
#define RAD_TO_DEG  57.2957795131f  // 180/π

extern const uint8_t hall_seq_clw[8];
extern const uint8_t hall_seq_ccw[8];

typedef struct MotorConst {
    uint8_t adc_u_id;
    uint8_t adc_v_id;
    uint8_t adc_w_id;
    GPIO_TypeDef*       Hall_GPIOx[3];
    uint16_t            Hall_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  htimx;
    uint32_t            TIM_CHANNEL_x[3];
    GPIO_TypeDef*       Coil_GPIOx[3];
    uint16_t            Coil_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  ELE_htimx;
} MotorConst;

typedef enum MotorCtrlMode {
    MOTOR_CTRL_120,
    MOTOR_CTRL_FOC,
} MotorCtrlMode;

typedef struct MotorParameter {
    const MotorConst    const_h;
    MotorCtrlMode       mode;
    float               rpm_fbk_hall;
    float               rpm_fbk_htim;
    uint8_t             exti_hall_last;         // GPIO trigger
    volatile uint8_t    exti_hall_curt;         // GPIO trigger
    volatile uint16_t   exti_hall_cnt;          // GPIO trigger
    volatile float      hall_angle_acc;         // 霍爾累積角度基準 每次霍爾相位切換時 馬達轉+60角度
    uint8_t             pwm_hall_last;          // PWM last hall record
    volatile uint16_t   pwm_hall_acc;           // PWM hall record total
    volatile float      pwm_per_it_angle_itpl;  // PWM 中斷應補角度 (Angle Interpolation)
    volatile float      pwm_it_angle_acc;
    uint16_t            pwm_count;
    uint16_t    spin_stop_acc;
    uint16_t    adc_u;
    uint16_t    adc_v;
    uint16_t    adc_w;
    volatile PI_CTRL    pi_speed;
    float               pi_speed_cmd;
    PI_CTRL             pi_Iq;
    PI_CTRL             pi_Id;
    CLARKE      clarke;
    PARK        park;
    IPARK       ipark;
    SVGENDQ     svgendq;
    float       elec_theta_rad;
    float       elec_theta_deg;
    float       svpwm_Vref;
    float       svpwm_T0;
    float       svpwm_T1;
    float       svpwm_T2;
    float       pwm_duty_u;
    float       pwm_duty_v;
    float       pwm_duty_w;
    bool        reverse;
} MotorParameter;
extern MotorParameter motor_0;

Result motor_hall_to_angle(volatile uint8_t hall, float *angle);
