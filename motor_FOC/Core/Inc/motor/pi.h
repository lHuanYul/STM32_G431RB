#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct PI_CTRL {
    float Ref;  // Input: reference set-point
    float Fbk;  // Input: feedback
    float Out;  // Output: controller output 
    float Kp;   // Parameter: proportional loop gain
    float Ki;   // Parameter: integral gain
    float Umax; // Parameter: upper saturation limit
    float Umin; // Parameter: lower saturation limit
    float up;   // Data: proportional term
    float ui;   // Data: integral term
    float v1;   // Data: pre-saturated controller output
    float i1;   // Data: integrator storage: ui(k-1)
    float w1;   // Data: saturation record: [u(k-1) - v(k-1)]
    float delta;
} PI_CTRL;

#define PI_CONTROLLER_DEFAULTS { \
    0f,     \
    0f,     \
    0f,     \
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

#ifndef EPS_EQ
#define EPS_EQ 1e-12f
#endif

Result PI_run(volatile PI_CTRL *pi);
