#include "motor/park.h"

inline void PARK_run(volatile PARK *park)
{
    park->Ds = (park->Alpha * park->Cosine) + (park->Beta  * park->Sine);
    park->Qs = (park->Beta  * park->Cosine) - (park->Alpha * park->Sine);
}

inline void IPARK_run(volatile IPARK *ipark)
{
    ipark->Alpha = (ipark->Vdref * ipark->Cosine) - (ipark->Vqref * ipark->Sine);
    ipark->Beta  = (ipark->Vqref * ipark->Cosine) + (ipark->Vdref * ipark->Sine);
}
