#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "connectivity/fdcan/basic.h"

Result fdcan_pkt_ist_read(FdcanPkt* pkt);
Result fdcan_pkt_rcv_read(FdcanPkt* pkt);
