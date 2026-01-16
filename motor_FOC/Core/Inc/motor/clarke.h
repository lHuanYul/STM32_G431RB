#pragma once

#include "main/config.h"
#include "HY_MOD/main/fn_state.h"

typedef struct CLARKE {
    float32_t As;       // Input: phase-a stator variable
    float32_t Bs;       // Input: phase-b stator variable
    float32_t Cs;       // Input: phase-c stator variable  
    float32_t Alpha;    // Output: stationary d-axis stator variable 
    float32_t Beta;     // Output: stationary q-axis stator variable
} CLARKE;

void CLARKE_run_ideal(CLARKE *clarke);
void CLARKE_run_nideal(CLARKE *clarke);
