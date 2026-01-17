#include "main/fdcan.h"

FdcanPktPool fdcan_pkt_pool;

static FdcanPkt* trsm_pkt_buf[FDCAN_TRSM_BUF_CAP];
FdcanPktBuf fdcan_trsm_pkt_buf = {
    .buf = trsm_pkt_buf,
    .cap = FDCAN_TRSM_BUF_CAP,
};

static FdcanPkt* recv_pkt_buf[FDCAN_RECV_BUF_CAP];
FdcanPktBuf fdcan_recv_pkt_buf = {
    .buf = recv_pkt_buf,
    .cap = FDCAN_RECV_BUF_CAP,
};

FdcanParametar fdcan_h = {
    .const_h = {
        .hfdcanx = &hfdcan1,
        // .rx = { .GPIOx = , .GPIO_Pin_x = },
        // .tx = { .GPIOx = , .GPIO_Pin_x = },
    },
};
