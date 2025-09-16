#pragma once

#include "main/config.h"

typedef struct CLARKE {
    float As;       // Input: phase-a stator variable
    float Bs;       // Input: phase-b stator variable
    float Cs;       // Input: phase-c stator variable  
    float Alpha;    // Output: stationary d-axis stator variable 
    float Beta;     // Output: stationary q-axis stator variable
} CLARKE;

//  1/sqrt(3) = 0.57735026918963
#define  ONEbySQRT3   0.57735026918963    /* 1/sqrt(3) */

// Clarke transform macro (with 2 currents)
//==========================================
#define CLARKE_MACRO(v)                         \
    v.Alpha = v.As;                             \
    v.Beta = (v.As + v.Bs * 2.0f) * ONEbySQRT3;
