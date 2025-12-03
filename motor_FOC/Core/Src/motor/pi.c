#include "motor/pi.h"
#include "motor/basic.h"

void PI_run(PI_CTRL *pi)
{
    pi->Error = pi->reference - pi->feedback;
    pi->Term_p = pi->Kp * pi->Error;
    if (pi->out == pi->out_origin)
    {
        pi->Term_i = pi->Term_i_last + pi->Ki * pi->Term_p;
        // pi->Term_i = pi->Term_i_last + pi->Ki * pi->Error;
    }
    else
    {
        pi->Term_i = pi->Term_i_last;
    }
    pi->Term_i_last = pi->Term_i;
    pi->out_origin = pi->Term_p + pi->Term_i;
    pi->out = pi->out_origin;
    VAR_CLAMPF(pi->out, pi->min, pi->max);
    pi->saturation = (pi->out != pi->out_origin) ? true : false;
}
