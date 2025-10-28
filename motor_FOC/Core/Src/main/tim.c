#include "main/tim.h"

uint32_t tim_clk_APB1, tim_clk_APB2;

inline void INIT_OWN_TIM(void)
{
    tim_clk_APB1 = HAL_RCC_GetPCLK1Freq();
    tim_clk_APB2 = HAL_RCC_GetPCLK2Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) tim_clk_APB1 *= 2U;
    if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1) tim_clk_APB2 *= 2U;
}
