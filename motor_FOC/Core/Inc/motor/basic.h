#pragma once

#include "main/config.h"
#include "motor/pi.h"
#include "motor/clarke.h"
#include "motor/park.h"
#include "motor/svgendq.h"

extern const uint8_t hall_seq_clw[8];
extern const uint8_t hall_seq_ccw[8];

#define MS_HIGH_PASS   1
#define MS_NONE_PASS   0
#define MS_LOW_PASS    -1
extern const int8_t motor_sequence[6][3];

typedef struct MotorConst {
    uint8_t adc_u_id;
    uint8_t adc_v_id;
    uint8_t adc_w_id;
    GPIO_TypeDef*       Hall_GPIOx[3];
    uint16_t            Hall_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  PWM_htimx[3];
    uint32_t            PWM_TIM_CHANNEL_x[3];
    // GPIO_TypeDef*       Coil_GPIOx[3];
    // uint16_t            Coil_GPIO_Pin_x[3];
    TIM_HandleTypeDef*  ELE_htimx;
} MotorConst;

typedef struct MotorParameter {
    const MotorConst const_h;
    volatile PI_CONTROLLER  pi_speed;
    volatile float          pi_speed_cmd;
    volatile PI_CONTROLLER  pi_Iq;
    volatile PI_CONTROLLER  pi_Id;
    volatile uint8_t    gpio_hall_last;     // GPIO trigger
    volatile uint8_t    gpio_hall_current;  // GPIO trigger
    volatile float      gpio_angle_current; // Transfer from gpio_hall_current
    volatile float      gpio_angle_acc;
    volatile uint8_t    pwm_hall_last;      // PWM last hall record
    volatile uint16_t   pwm_hall_acc;       // PWM hall record total
    volatile uint16_t   pwm_hall_same_acc;
    volatile float      pwm_it_angle;
    volatile float      pwm_it_angle_acc;
    volatile uint16_t   pwm_count;
    volatile uint16_t   adc_u;
    volatile uint16_t   adc_v;
    volatile uint16_t   adc_w;
    volatile CLARKE     clarke;
    volatile PARK       park;
    volatile IPARK      ipark;
    volatile SVGENDQ    svgendq;
    volatile float      electric_theta_rad;
    volatile float      electric_theta_deg;
    volatile float      SVPWM_Vref;
    volatile float      pwm_duty_u;
    volatile float      pwm_duty_v;
    volatile float      pwm_duty_w;
    uint16_t    pwm_duty;       // 0-100
    uint16_t    pwm_compare;    // Transfer from pwm_duty
    bool        reverse;
} MotorParameter;

extern MotorParameter motor_0;
