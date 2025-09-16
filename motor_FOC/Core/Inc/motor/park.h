#pragma once

#include "main/config.h"

typedef struct PARK {
    float Alpha;    // Input: stationary d-axis stator variable 
    float Beta;     // Input: stationary q-axis stator variable 
    float Angle;    // Input: rotating angle (pu) 
    float Ds;       // Output: rotating d-axis stator variable 
    float Qs;       // Output: rotating q-axis stator variable
    float Sine;
    float Cosine;
} PARK;

#define PARK_MACRO(v)                               \
	v.Ds = v.Alpha * v.Cosine + v.Beta  * v.Sine;   \
    v.Qs = v.Beta  * v.Cosine - v.Alpha * v.Sine;

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

#define IPARK_MACRO(v)                                      \
    v.Alpha = (v.Vdref * v.Cosine) - (v.Vqref * v.Sine);    \
    v.Beta  = (v.Vqref * v.Cosine) + (v.Vdref * v.Sine);
