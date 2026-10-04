#include "main/main.h"

#include "tim.h"
#include "HY_MOD/main/tim.h"
#include "HY_MOD/fdcan/basic.h"

static FdcanPkt tx[FDCAN_TRSM_BUF_CAP];
static FdcanPkt rx[FDCAN_RECV_BUF_CAP];
FdcanParametar fdcan_h = {
    .const_h = {
        .hfdcanx    = &hfdcan1,
        .htimx      = &htim16,
        .tim_clk    = &tim_clk_APB2,
    },
    .tx_buf = {
        .buf = tx,
        .cap = FDCAN_TRSM_BUF_CAP,
    },
    .rx_buf = {
        .buf = rx,
        .cap = FDCAN_RECV_BUF_CAP,
    },
};
