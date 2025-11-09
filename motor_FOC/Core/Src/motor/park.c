#include "motor/park.h"

// Id = Ialpha * cos(theta) + Ibata * sin(theta)
// Iq = Ibata * cos(theta) - Ialpha * sin(theta)
inline void PARK_run(volatile PARK *park)
{
    park->Ds = (park->Alpha * park->Cosine) + (park->Beta  * park->Sine);
    park->Qs = (park->Beta  * park->Cosine) - (park->Alpha * park->Sine);
}

// Valpha = Vd * cos(theta) - Vq * sin(theta)
// Vbata = Vd * sin(theta) + Vq * cos(theta)
inline void IPARK_run(volatile IPARK *ipark)
{
    ipark->Alpha = (ipark->Vdref * ipark->Cosine) - (ipark->Vqref * ipark->Sine);
    ipark->Beta  = (ipark->Vqref * ipark->Cosine) + (ipark->Vdref * ipark->Sine);
}
