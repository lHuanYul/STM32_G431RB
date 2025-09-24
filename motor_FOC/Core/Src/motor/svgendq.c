#include "motor/svgendq.h"

Result SVGEN_run(volatile SVGENDQ *svgq)
{
    float t1 = svgq->Ubeta * 0.5f;
    float t2 = 0.8660254f * svgq->Ualpha;   // sqrt(3)/2
    // Inverse clarke transformation
    svgq->Va = svgq->Ubeta;
    svgq->Vb = -t1 + t2;
    svgq->Vc = -t1 - t2;
    // 60 degree Sector determination
    if (svgq->Va > 0.0f) svgq->Sector = 2;
    if (svgq->Vb > 0.0f) svgq->Sector += 4;
    if (svgq->Vc > 0.0f) svgq->Sector += 1;
    // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
    return RESULT_OK(NULL);
}
