#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct PARK {
    float Alpha;    // Input: stationary d-axis stator variable 
    float Beta;     // Input: stationary q-axis stator variable 
    float Angle;    // Input: rotating angle (pu) 
    float Ds;       // Output: rotating d-axis stator variable 
    float Qs;       // Output: rotating q-axis stator variable
    float Sine;
    float Cosine;
} PARK;

Result PARK_run(volatile PARK *park);

#define PARK_MACRO_Swap(v)                          \
	v.Ds = v.Beta  * v.Cosine + v.Alpha * v.Sine;   \
    v.Qs = v.Alpha * v.Cosine - v.Beta  * v.Sine;

typedef struct IPARK {
    float Alpha;    // Output: stationary d-axis stator variable
    float Beta;     // Output: stationary q-axis stator variable
    float Angle;    // Input: rotating angle (pu)
    float Vdref;    // Input: rotating d-axis stator variable
    float Vqref;    // Input: rotating q-axis stator variable
    float Sine;     // Input: Sine term
    float Cosine;   // Input: Cosine term
} IPARK;

Result IPARK_run(volatile IPARK *ipark);
