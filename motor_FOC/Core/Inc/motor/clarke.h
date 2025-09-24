#pragma once

#include "main/config.h"
#include "main/fn_state.h"

//  1/sqrt(3) = 0.57735026918963
#define ONEbySQRT3  0.57735026918963    /* 1/sqrt(3) */
#define DIV_1_3     (1.0f / 3.0f)
#define DIV_2_3     (2.0f / 3.0f)

typedef struct CLARKE {
    float As;       // Input: phase-a stator variable
    float Bs;       // Input: phase-b stator variable
    float Cs;       // Input: phase-c stator variable  
    float Alpha;    // Output: stationary d-axis stator variable 
    float Beta;     // Output: stationary q-axis stator variable
} CLARKE;

Result CLARKE_run_ideal(volatile CLARKE *clarke);
Result CLARKE_run_nideal(volatile CLARKE *clarke);
