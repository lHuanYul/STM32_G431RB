#include "connectivity/fdcan/pkt_read.h"
#include "connectivity/cmds.h"
#include "main/variable_cal.h"

#ifdef MCU_MOTOR_CTRL
#include "motor/basic.h"

Result fdcan_pkt_ist_read(FdcanPkt *pkt)
{
    uint8_t code;
    switch (pkt->id)
    {
        case FDCAN_WHEEL_SET_ID:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
            switch (code)
            {
                case CMD_WHEEL_B0_COAST:
                {
                    motor_set_rotate_mode(&motor_h, MOTOR_ROT_COAST);
                    return RESULT_OK(NULL);
                }
                case CMD_WHEEL_B0_SET_SPD:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
                    if (code) code = 1;
                    uint8_t spd_u8[sizeof(float32_t)];
                    memcpy(spd_u8, pkt->data + 2, sizeof(float32_t));
                    motor_set_rotate_mode(&motor_h, MOTOR_ROT_NORMAL);
                    motor_set_speed(&motor_h, code, var_u8_to_f32_be(spd_u8));
                    return RESULT_OK(NULL);
                }
                case CMD_WHEEL_B0_LOCK:
                {
                    motor_set_rotate_mode(&motor_h, MOTOR_ROT_LOCK_PRE);
                    return RESULT_OK(NULL);
                }
                default: break;
            }
        }
        default: break;
    }
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}
#endif

#ifdef MCU_VEHICLE_MAIN
#include "vehicle/main.h"

static Result motor_pkt(FdcanPkt *pkt, MotorSet *motor)
{
    if (pkt->len != 1 + sizeof(float32_t)) return RESULT_ERROR(RES_ERR_NOT_FOUND);
    motor->mode = pkt->data[0];
    motor->reverse = pkt->data[1];
    uint8_t spd_u8[sizeof(float32_t)];
    memcpy(spd_u8, pkt->data + 2, sizeof(float32_t));
    motor->value = var_u8_to_f32_be(spd_u8);
    return RESULT_OK(NULL);
}

Result fdcan_pkt_ist_read(FdcanPkt *pkt)
{
    uint8_t code;
    switch (pkt->id)
    {
        case CAN_ID_WHEEL_LEFT_SPD_FBK:
        {
            return motor_pkt(pkt, &vehicle_h.motor_left_fbk);
        }
        case CAN_ID_WHEEL_RIGHT_SPD_FBK:
        {
            return motor_pkt(pkt, &vehicle_h.motor_right_fbk);
        }
        case CAN_ID_HALL_ALL:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
            if (
                code != ADC_HALL_STATE_NONE &&
                code != ADC_HALL_STATE_ON_MAG
            ) return RESULT_ERROR(RES_ERR_NOT_FOUND);
            vehicle_h.hall_front = code;
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
            if (
                code != ADC_HALL_STATE_NONE &&
                code != ADC_HALL_STATE_ON_MAG
            ) return RESULT_ERROR(RES_ERR_NOT_FOUND);
            vehicle_h.hall_left = code;
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
            if (
                code != ADC_HALL_STATE_NONE &&
                code != ADC_HALL_STATE_ON_MAG
            ) return RESULT_ERROR(RES_ERR_NOT_FOUND);
            vehicle_h.hall_right = code;
            break;
        }
        case CAN_ID_US_SENSOR:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
            if (
                code != USS_STATUS_SAVE &&
                code != USS_STATUS_WARNING &&
                code != USS_STATUS_DANGER
            ) return RESULT_ERROR(RES_ERR_NOT_FOUND);
            vehicle_h.us_sensor = code;
            break;
        }
        default: break;
    }
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}

Result fdcan_pkt_rcv_read(FdcanPkt *pkt)
{
    uint8_t code;
    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
    
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}
#endif
