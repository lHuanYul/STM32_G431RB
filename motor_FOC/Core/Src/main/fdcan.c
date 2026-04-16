#include "main/main.h"

#include "tim.h"
#include "HY_MOD/main/tim.h"

static FdcanPkt* trsm_pkt_buf[FDCAN_TRSM_BUF_CAP];
static FdcanPkt* recv_pkt_buf[FDCAN_RECV_BUF_CAP];
FdcanParametar fdcan_h = {
    .const_h = {
        .hfdcanx    = &hfdcan1,
        .htimx      = &htim16,
        .tim_clk    = &tim_clk_APB2,
    },
    .pool = {},
    .trsm_buf = {
        .buf = trsm_pkt_buf,
        .cap = FDCAN_TRSM_BUF_CAP,
    },
    .recv_buf = {
        .buf = recv_pkt_buf,
        .cap = FDCAN_RECV_BUF_CAP,
    },
};
