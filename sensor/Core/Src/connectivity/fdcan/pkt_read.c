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
            motor_alive(&motor_h);
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
            switch (code)
            {
                case CMD_WHEEL_B0_COAST:
                {
                    motor_set_rotate_mode(&motor_h, MOTOR_ROT_COAST);
                    return RESULT_OK(NULL);
                }
                case CMD_WHEEL_B0_NORMAL:
                {
                    if (pkt->len < 6) break;
                    motor_set_rotate_mode(&motor_h, MOTOR_ROT_NORMAL);
                    code = (pkt->data[1]) ? 1 : 0;
                    uint8_t spd_u8[sizeof(float32_t)];
                    memcpy(spd_u8, pkt->data + 2, sizeof(float32_t));
                    motor_set_speed(&motor_h, code, var_u8_to_f32_be(spd_u8));
                    return RESULT_OK(NULL);
                }
                case CMD_WHEEL_B0_LOCK:
                {
                    motor_set_rotate_mode(&motor_h, MOTOR_ROT_LOCK);
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

static Result motor_pkt(FdcanPkt *pkt, MotorParameter *motor)
{
    motor->alive_tick = HAL_GetTick();
    if (pkt->len != 1 + sizeof(float32_t)) return RESULT_ERROR(RES_ERR_NOT_FOUND);
    motor->mode_fbk = pkt->data[0];
    motor->rev_fbk = pkt->data[1];
    uint8_t spd_u8[sizeof(float32_t)];
    memcpy(spd_u8, pkt->data + 2, sizeof(float32_t));
    motor->value_fbk = var_u8_to_f32_be(spd_u8);
    return RESULT_OK(NULL);
}

static void hall_read(uint8_t code, VehicleHall *hall)
{
    if (
        code != ADC_HALL_STATE_NONE &&
        code != ADC_HALL_STATE_ON_MAG
    ) code = ADC_HALL_STATE_UNK;
    hall->state = code;
    hall->alive_tick = HAL_GetTick();
}

static void uss_read(uint8_t code, VehicleUSS *uss)
{
    if (
        code != USS_STATUS_SAVE &&
        code != USS_STATUS_WARNING &&
        code != USS_STATUS_DANGER
    ) code = USS_STATUS_UNK;
    uss->state = code;
    uss->alive_tick = HAL_GetTick();
}

Result fdcan_pkt_ist_read(FdcanPkt *pkt)
{
    uint8_t code;
    switch (pkt->id)
    {
        case CAN_ID_WHEEL_LEFT_SPD_FBK:
        {
            return motor_pkt(pkt, &vehicle_h.motor_left);
        }
        case CAN_ID_WHEEL_RIGHT_SPD_FBK:
        {
            return motor_pkt(pkt, &vehicle_h.motor_right);
        }
        case CAN_ID_HALL_ALL:
        {
            if (!fdcan_pkt_check_len(pkt, 4)) break;
            hall_read(pkt->data[0], &vehicle_h.hall_front);
            hall_read(pkt->data[1], &vehicle_h.hall_left);
            hall_read(pkt->data[2], &vehicle_h.hall_right);
            uss_read(pkt->data[3], &vehicle_h.us_sensor);
            return RESULT_OK(NULL);
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

#ifdef MCU_SENSOR
Result fdcan_pkt_ist_read(FdcanPkt *pkt)
{
    uint8_t code;
    switch (pkt->id)
    {
        
        default: break;
    }
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}
#endif
