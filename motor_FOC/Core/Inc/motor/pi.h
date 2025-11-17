#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct PI_CTRL {
    float32_t Ref;  // Input: reference set-point
    float32_t Fbk;  // Input: feedback
    float32_t Out;  // Output: controller output 
    float32_t Kp;   // Parameter: proportional loop gain 比例增益
    float32_t Ki;   // Parameter: integral gain 積分增益
    float32_t Umax; // Parameter: upper saturation limit
    float32_t Umin; // Parameter: lower saturation limit
    float32_t Dlta; // Data: Ref - Fbk
    float32_t up;   // Data: proportional term 比例項 Kp * Dlta
    float32_t ui;   // Data: integral term 積分項
    float32_t v1;   // Data: pre-saturated controller output 飽和前的控制輸出
    float32_t i1;   // Data: integrator storage: ui(k-1) 積分器狀態
    float32_t w1;   // Data: saturation record: [u(k-1) - v(k-1)] 飽和誤差記錄
} PI_CTRL;

#define PI_CONTROLLER_DEFAULTS { \
    0.0f,   \
    0.0f,   \
    0.0f,   \
    1.0f,   \
    0.0f,   \
    1.0f,   \
    -1.0f,  \
    0.0f,   \
    0.0f,   \
    0.0f,   \
    0.0f,   \
    1.0f,   \
    0.0f    \
}

void PI_run(PI_CTRL *pi);
