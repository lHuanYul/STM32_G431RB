#include "motor/pi.h"
#include "motor/basic.h"

void PI_run(PI_CTRL *pi)
{
    pi->Delta = pi->ref - pi->fbk;
    pi->Up = pi->Kp * pi->Delta;
    if (pi->out == pi->out_ori)
    {
        pi->Ui = pi->Uil + pi->Ki * pi->Up;
    }
    else
    {
        pi->Ui = pi->Uil;
    }
    pi->Uil = pi->Ui;
    pi->out_ori = pi->Up + pi->Ui;
    pi->out = var_clampf(pi->out_ori, pi->min, pi->max);
    pi->sat = (pi->out == pi->out_ori) ? 1 : 0;
}
