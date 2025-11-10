#pragma once

#include "main/config.h"
#include "motor/pi.h"
#include "motor/clarke.h"
#include "motor/park.h"
#include "motor/svgendq.h"
#include "main/variable_cal.h"
#include "analog/adc1/basic.h"
#include "cordic.h"

extern const uint8_t hall_seq_clw[8];
extern const uint8_t hall_seq_ccw[8];

typedef struct MotorConst
{
    // HALL PIN
    GPIO_TypeDef        *Hall_GPIOx[3];
    uint16_t            Hall_GPIO_Pin_x[3];
    // PWM timer
    TIM_HandleTypeDef   *PWM_htimx;
    uint32_t            PWM_TIM_CHANNEL_x[3];
    // deg control L
    GPIO_TypeDef        *Coil_GPIOx[3];
    uint16_t            Coil_GPIO_Pin_x[3];
    // 轉速計時器
    TIM_HandleTypeDef   *SPD_htimx;
    uint32_t            *SPD_tim_clk;
    // FOC 20kHz timer
    TIM_HandleTypeDef   *IT20k_htimx;
    uint32_t            *IT20k_tim_clk;
} MotorConst;

typedef enum MotorCtrlMode
{
    MOTOR_CTRL_120,
    MOTOR_CTRL_FOC,
} MotorCtrlMode;

typedef struct MotorParameter
{
    float32_t           dbg_tim_it_freq;
    uint16_t            dbg_tim_it_cnt;
    // 常數
    const MotorConst    const_h;
    // 霍爾間隔 → 輸出軸轉速(RPM) 轉換常數
    // RPM = [ELE_tim_f * 60] / [6 × (POLE/2) × GEAR × htim_cnt]
    float32_t           tfm_rpm_fbk;
    // PWM 週期 → 電角度內插轉換常數
    // Δθ_elec(rad) = [ (TIM_tim_t * ARR) / ELE_tim_t ] × (π/3) / htim_cnt
    float32_t           tfm_tim_it_angle_itpl;
    // 馬達控制方式
    MotorCtrlMode       mode;
    // 目前 RPM
    float32_t           rpm_fbk;
    // 上次霍爾相位
    uint8_t             exti_hall_last;
    // 目前霍爾相位(546231)
    volatile uint8_t    exti_hall_curt;
    // 目前霍爾相位(rad)
    volatile float32_t  exti_hall_angal;
    //
    volatile uint8_t    exti_hall_cnt;
    // 霍爾累積角度基準
    // 每次霍爾相位切換時 馬達轉+60角度
    // volatile float32_t  hall_angle_acc;
    // 上次 FOC 霍爾相位
    uint8_t             chk_hall_last;
    // FOC 霍爾相位和
    // chk_hall_total = chk_hall_last*10 + exti_hall_angal;
    volatile uint16_t   chk_hall_total;
    // FOC 應補角度 (Angle Interpolation)
    volatile float32_t  tim_angle_itpl;
    // FOC 角度累積插值
    // tim_angle_acc += tim_angle_itpl; 過一霍爾中斷後重置
    volatile float32_t  tim_angle_acc;
    // 停轉計數器
    uint16_t            stop_spin_acc;
    // 電流 ADC
    CURRENT_ADC         *adc_a;
    // 電流 ADC
    CURRENT_ADC         *adc_b;
    // 電流 ADC
    CURRENT_ADC         *adc_c;
    // 0: ccw / 1: clw
    bool                reverse;

    volatile PI_CTRL    pi_speed;
    float32_t           pi_speed_cmd;
    PI_CTRL             pi_Iq;
    PI_CTRL             pi_Id;
    CLARKE              clarke;
    PARK                park;
    IPARK               ipark;
    SVGENDQ             svgendq;
    float32_t           elec_theta_rad;
    float32_t           pwm_duty_120;
    float32_t           pwm_duty_u;
    float32_t           pwm_duty_v;
    float32_t           pwm_duty_w;
} MotorParameter;
extern MotorParameter motor_h;

float32_t clampf(float32_t val, float32_t min, float32_t max);
float32_t wrap_positive(float32_t x, float32_t value);
float32_t wrap_pi_p_n(float32_t x, float32_t value);
float32_t fast_fabsf(float32_t x);

