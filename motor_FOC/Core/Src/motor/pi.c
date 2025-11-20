#include "motor/pi.h"
#include "motor/basic.h"

void PI_run(PI_CTRL *pi)
{
    pi->Err = pi->ref - pi->fbk;
    pi->Up = pi->Kp * pi->Err;
    if (pi->out == pi->ori)
    {
        pi->Ui = pi->Uil + pi->Ki * pi->Up;
    }
    else
    {
        pi->Ui = pi->Uil;
    }
    pi->Uil = pi->Ui;
    pi->ori = pi->Up + pi->Ui;
    pi->out = var_clampf(pi->ori, pi->min, pi->max);
    pi->sat = (pi->out == pi->ori) ? 1 : 0;
}
