#include "connectivity/fdcan/pkt_write.h"
#include "connectivity/fdcan/pkt_read.h"
#include "main/variable_cal.h"

static float ftest = 0.0;
Result fdcan_pkt_write_test(FdcanPkt* pkt)
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

Result fdcan_pkt_write_spd_fbk(FdcanPkt* pkt)
{
    if (pkt == NULL) return RESULT_ERROR(RES_ERR_MEMORY_ERROR);
    MotorParameter *motor = &motor_h;
    pkt->id = FDCAN_WHEEL_FBK_ID;
    RESULT_CHECK_HANDLE(fdcan_pkt_set_len(pkt, 1 + sizeof(motor->rpm_feedback.value)));
    pkt->data[0] = (uint8_t)motor->mode_rotate;
    var_f32_to_u8_be(motor->rpm_feedback.value, pkt->data + 1);
    return RESULT_OK(pkt);
}
#endif

#ifdef MCU_VEHICLE_MAIN
Result fdcan_pkt_write_motor(FdcanPkt* pkt, uint16_t id, uint8_t mode, float32_t spd)
{
    if (pkt == NULL) return RESULT_ERROR(RES_ERR_MEMORY_ERROR);
    pkt->id = id;
    pkt->data[0] = mode;
    RESULT_CHECK_HANDLE(pkt_data_write_f32(pkt, 1, spd));
    RESULT_CHECK_HANDLE(fdcan_pkt_set_len(pkt, 1 + sizeof(float32_t)));
    return RESULT_OK(pkt);
}
#endif
