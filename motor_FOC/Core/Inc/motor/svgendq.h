#pragma once

#include "main/config.h"

typedef struct SVGENDQ	{
    float       Ualpha; // Input: reference alpha-axis phase voltage 
    float       Ubeta;  // Input: reference beta-axis phase voltage 
    float       Va;     // Output: reference phase-a switching function		
    float       Vb;     // Output: reference phase-b switching function 
    float       Vc;     // Output: reference phase-c switching function
    uint16_t    Sector; // Sector is treated as Q0 - independently with global Q
} SVGENDQ;

float temp_sv1,temp_sv2;
#define SVGEN_MACRO(v)                                              \
	v.Sector = 0;                                                   \
	temp_sv1 = v.Ubeta / 2.0f;                                      \
	temp_sv2 = 0.8660254f * v.Ualpha;	/* 0.8660254 = sqrt(3)/2*/  \
    /* Inverse clarke transformation */                             \
	v.Va = v.Ubeta;                                                 \
	v.Vb = -temp_sv1 + temp_sv2;                                    \
	v.Vc = -temp_sv1 - temp_sv2;                                    \
    /* 60 degree Sector determination */                            \
	if (v.Va > 0.0f) v.Sector = 2;                                  \
	if (v.Vb > 0.0f) v.Sector = v.Sector+4;                         \
	if (v.Vc > 0.0f) v.Sector = v.Sector+1;                         \
    /* Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)*/
