#include "connectivity/fdcan/pkt_write.h"
#include "connectivity/fdcan/pkt_read.h"
#include "main/variable_cal.h"

static float ftest = 0.0;
Result fdcan_pkt_write_test(FdcanPkt *pkt)
{
    if (pkt == NULL) return RESULT_ERROR(RES_ERR_MEMORY_ERROR);
    pkt->id = FDCAN_TEST_ID;
    pkt->data[0] = 0x00;
    pkt->data[1] = 0xFF;
    var_f32_to_u8_be(ftest++, pkt->data + 2);
    pkt->len = 6;
    return RESULT_OK(pkt);
}

#ifdef MCU_MOTOR_CTRL
#include "motor/basic.h"

Result fdcan_pkt_write_spd_fbk(FdcanPkt *pkt)
{
    if (pkt == NULL) return RESULT_ERROR(RES_ERR_MEMORY_ERROR);
    MotorParameter *motor = &motor_h;
    pkt->id = FDCAN_WHEEL_FBK_ID;
    RESULT_CHECK_HANDLE(fdcan_pkt_set_len(pkt, 1 + sizeof(float32_t)));
    pkt->data[0] = motor->mode_rotate;
    pkt->data[1] = motor->rpm_feedback.reverse;
    var_f32_to_u8_be(motor->rpm_feedback.value, pkt->data + 2);
    return RESULT_OK(pkt);
}
#endif

#ifdef MCU_VEHICLE_MAIN
Result fdcan_pkt_write_motor(FdcanPkt *pkt, MotorParameter *motor)
{
    if (pkt == NULL) return RESULT_ERROR(RES_ERR_MEMORY_ERROR);
    pkt->id = motor->id;
    pkt->data[0] = motor->mode_ref;
    pkt->data[1] = motor->rev_ref;
    var_f32_to_u8_be((float32_t)motor->value_ref * motor->max, pkt->data + 2);
    RESULT_CHECK_HANDLE(fdcan_pkt_set_len(pkt, 2 + sizeof(float32_t)));
    return RESULT_OK(pkt);
}

Result fdcan_vehicle_motor_send(VehicleParameter *vehicle)
{
    FdcanPkt *pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_pkt_write_motor(pkt, &vehicle->motor_left);
    RESULT_CHECK_HANDLE(fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt));
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_pkt_write_motor(pkt, &vehicle->motor_right);
    RESULT_CHECK_HANDLE(fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt));
    return RESULT_OK(NULL);
}
#endif
