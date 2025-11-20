#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct PI_CTRL {
    float32_t   ref;    // Input: reference set-point
    float32_t   fbk;    // Input: feedback
    float32_t   out;    // Output: controller output
    float32_t   ori;    // Data: pre-saturated controller output 飽和前的控制輸出
    float32_t   max;
    float32_t   min;
    float32_t   Kp;     // Parameter: proportional loop gain 比例增益
    float32_t   Ki;     // Parameter: integral gain 積分增益 Ki*Ts/Kp
    float32_t   Err;    // Data: ref - Fbk
    float32_t   Up;     // Data: proportional term 比例項 Kp * Err
    float32_t   Ui;     // Data: integral term 積分項
    float32_t   Uil;    // Data: integrator storage: Ui(k-1) 積分器狀態
    bool        sat;    // Data: saturation record: [u(k-1) - v(k-1)] 飽和誤差記錄
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
