#include "motor/pi.h"
#include "motor/basic.h"

void PI_run(PI_CTRL *pi)
{
    pi->Dlta = pi->Ref - pi->Fbk;
    pi->up = pi->Kp * pi->Dlta;
    if (pi->Out == pi->v1)
    {
        pi->ui = pi->Ki * pi->up + pi->i1;
    }
    else
    {
        pi->ui = pi->i1;
    }
    pi->i1 = pi->ui;
    pi->v1 = pi->up + pi->ui;
    pi->Out = var_clampf(pi->v1, pi->Umin, pi->Umax);
    pi->w1 = (pi->Out == pi->v1) ? 1.0f : 0.0f;
}
