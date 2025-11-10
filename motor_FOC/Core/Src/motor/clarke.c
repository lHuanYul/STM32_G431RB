#include "motor/clarke.h"
#include "motor/basic.h"

// Ialpha = Ia
// Ibata = (Ia+2Ib)/(根號3)
inline void CLARKE_run_ideal(volatile CLARKE *clarke)
{
    clarke->Alpha = clarke->As;
    clarke->Beta = (clarke->As + clarke->Bs * 2.0f) * ONE_DIV_SQRT3;
}

// Ialpha = 2/3Ia - 1/3Ib - 1/3Ic
// Ibata = (根號3/3)Ib - (根號3/3)Ic
// (根號3/3) = 0.57735
inline void CLARKE_run_nideal(volatile CLARKE *clarke)
{
    clarke->Alpha = DIV_2_3 * clarke->As - DIV_1_3 * clarke->Bs - DIV_1_3 * clarke->Cs;
    clarke->Beta  = (clarke->Bs - clarke->Cs) * ONE_DIV_SQRT3;
}
