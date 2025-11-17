#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct PARK {
    float32_t Alpha;    // Input: stationary d-axis stator variable 
    float32_t Beta;     // Input: stationary q-axis stator variable 
    float32_t Angle;    // Input: rotating angle (pu) 
    float32_t Ds;       // Output: rotating d-axis stator variable 
    float32_t Qs;       // Output: rotating q-axis stator variable
    float32_t Sin;
    float32_t Cos;
} PARK;

void PARK_run(PARK *park);

typedef struct IPARK {
    float32_t Alpha;    // Output: stationary d-axis stator variable
    float32_t Beta;     // Output: stationary q-axis stator variable
    float32_t Angle;    // Input: rotating angle (pu)
    float32_t Vdref;    // Input: rotating d-axis stator variable
    float32_t Vqref;    // Input: rotating q-axis stator variable
    float32_t Sin;     // Input: Sine term
    float32_t Cos;   // Input: Cosine term
} IPARK;

void IPARK_run(IPARK *ipark);
