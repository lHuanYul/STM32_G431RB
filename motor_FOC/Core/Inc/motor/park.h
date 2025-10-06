#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct PARK {
    float32_t Alpha;    // Input: stationary d-axis stator variable 
    float32_t Beta;     // Input: stationary q-axis stator variable 
    float32_t Angle;    // Input: rotating angle (pu) 
    float32_t Ds;       // Output: rotating d-axis stator variable 
    float32_t Qs;       // Output: rotating q-axis stator variable
    float32_t Sine;
    float32_t Cosine;
} PARK;

void PARK_run(volatile PARK *park);

#define PARK_MACRO_Swap(v)                          \
	v.Ds = v.Beta  * v.Cosine + v.Alpha * v.Sine;   \
    v.Qs = v.Alpha * v.Cosine - v.Beta  * v.Sine;

typedef struct IPARK {
    float32_t Alpha;    // Output: stationary d-axis stator variable
    float32_t Beta;     // Output: stationary q-axis stator variable
    float32_t Angle;    // Input: rotating angle (pu)
    float32_t Vdref;    // Input: rotating d-axis stator variable
    float32_t Vqref;    // Input: rotating q-axis stator variable
    float32_t Sine;     // Input: Sine term
    float32_t Cosine;   // Input: Cosine term
} IPARK;

void IPARK_run(volatile IPARK *ipark);
