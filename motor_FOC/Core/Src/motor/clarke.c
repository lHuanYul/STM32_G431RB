#include "motor/clarke.h"

Result CLARKE_run_ideal(volatile CLARKE *clarke)
{
    clarke->Alpha = clarke->As;
    clarke->Beta = (clarke->As + clarke->Bs * 2.0f) * ONEbySQRT3;
    return RESULT_OK(NULL);
}

Result CLARKE_run_nideal(volatile CLARKE *clarke)
{
    clarke->Alpha = DIV_2_3 * clarke->As - DIV_1_3 * clarke->Bs - DIV_1_3 * clarke->Cs;
    clarke->Beta  = ONEbySQRT3 * (clarke->Bs - clarke->Cs);
    return RESULT_OK(NULL);
}
