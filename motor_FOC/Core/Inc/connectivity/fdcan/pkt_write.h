#pragma once

#include "main/config.h"
#include "main/fn_state.h"
#include "connectivity/fdcan/basic.h"

Result fdcan_pkt_write_test(FdcanPkt *pkt);

#ifdef MCU_MOTOR_CTRL
#include "motor/basic.h"

Result fdcan_motor_send(MotorParameter *motor);
#endif

#ifdef MCU_VEHICLE_MAIN
#include "vehicle/basic.h"

Result fdcan_pkt_write_motor(FdcanPkt *pkt, MotorParameter *motor);
Result fdcan_vehicle_motor_send(VehicleParameter *vehicle);
#endif
