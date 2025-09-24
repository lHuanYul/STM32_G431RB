#pragma once

#include "main/config.h"
#include "main/fn_state.h"

typedef struct SVGENDQ	{
    float       Ualpha; // Input: reference alpha-axis phase voltage 
    float       Ubeta;  // Input: reference beta-axis phase voltage 
    float       Va;     // Output: reference phase-a switching function		
    float       Vb;     // Output: reference phase-b switching function 
    float       Vc;     // Output: reference phase-c switching function
    uint16_t    Sector; // Sector is treated as Q0 - independently with global Q
} SVGENDQ;

Result SVGEN_run(volatile SVGENDQ *svgq);
