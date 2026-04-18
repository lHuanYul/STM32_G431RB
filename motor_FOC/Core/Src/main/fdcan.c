#include "main/main.h"

#include "tim.h"
#include "HY_MOD/main/tim.h"

static FdcanPkt fdcan_tx_pkts[FDCAN_TRSM_BUF_CAP];
static FdcanPkt fdcan_rx_pkts[FDCAN_RECV_BUF_CAP];
FdcanParametar fdcan_h = {
    .const_h = {
        .hfdcanx    = &hfdcan1,
        .htimx      = &htim16,
        .tim_clk    = &tim_clk_APB2,
    },
    .tx_buf = {
        .buf = fdcan_tx_pkts,
        .cap = FDCAN_TRSM_BUF_CAP,
    },
    .rx_buf = {
        .buf = fdcan_rx_pkts,
        .cap = FDCAN_RECV_BUF_CAP,
    },
};
