#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "connectivity/fdcan/basic.h"

Result fdcan_pkt_write_test(FdcanPkt* pkt);

#ifdef MCU_MOTOR_CTRL
Result fdcan_pkt_write_spd_fbk(FdcanPkt* pkt);
#endif

#ifdef MCU_VEHICLE_MAIN
Result fdcan_pkt_write_motor(FdcanPkt* pkt, uint16_t id, uint8_t mode, float32_t spd);
#endif
