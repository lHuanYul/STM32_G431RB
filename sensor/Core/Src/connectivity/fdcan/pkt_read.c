#include "connectivity/fdcan/pkt_read.h"
#include "connectivity/cmds.h"
#include "main/variable_cal.h"

__weak Result fdcan_pkt_ist_read_inner(FdcanPkt* pkt)
{
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}

__weak Result fdcan_pkt_rcv_read_inner(FdcanPkt* pkt)
{
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}

#ifdef PRINCIPAL_PROGRAM
#include "vehicle/basic.h"
#include "motor/main.h"

static inline Result fdcan_pkt_ist_read_inner(FdcanPkt* pkt)
{
    uint8_t code;
    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
    switch (code)
    {
        case CMD_WHEEL_B0_CONTROL:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
            switch (code)
            {
                case CMD_WHEEL_B1_LEFT:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    MotorParameter* motor = &motor_left;
                    switch (code)
                    {
                        case CMD_WHEEL_B2_MODE:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            switch (code)
                            {
                                case CMD_WHEEL_B3_CONTROL:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    motor_set_state(motor, MOTOR_STATE_CONTROL);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_FREE:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    motor_set_state(motor, MOTOR_STATE_FREE);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_SLOW:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    motor_set_state(motor, MOTOR_STATE_SLOW);
                                    return RESULT_OK(NULL);
                                }
                                default: break;
                            }
                            break;
                        }
                        case CMD_WHEEL_B2_MOTION:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            switch (code)
                            {
                                case CMD_WHEEL_B3_CONTROL:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    vehicle_set_motion(VEHICLE_MOTION_UNKNOWN);
                                    motor_set_direct(motor, MOTOR_DIRECTION_STOP);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_FREE:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    vehicle_set_motion(VEHICLE_MOTION_UNKNOWN);
                                    motor_set_direct(motor, MOTOR_DIRECTION_CCLW);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_SLOW:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    vehicle_set_motion(VEHICLE_MOTION_UNKNOWN);
                                    motor_set_direct(motor, MOTOR_DIRECTION_CLW);
                                    return RESULT_OK(NULL);
                                }
                                default: break;
                            }
                            break;
                        }
                        case CMD_WHEEL_B2_SPEED:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            motor_set_rps_pcn(motor, code);
                            return RESULT_OK(NULL);
                        }
                        default: break;
                    }
                    break;
                }
                case CMD_WHEEL_B1_RIGHT:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    MotorParameter* motor = &motor_right;
                    switch (code)
                    {
                        case CMD_WHEEL_B2_MODE:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            switch (code)
                            {
                                case CMD_WHEEL_B3_CONTROL:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    motor_set_state(motor, MOTOR_STATE_CONTROL);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_FREE:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    motor_set_state(motor, MOTOR_STATE_FREE);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_SLOW:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    motor_set_state(motor, MOTOR_STATE_SLOW);
                                    return RESULT_OK(NULL);
                                }
                                default: break;
                            }
                            break;
                        }
                        case CMD_WHEEL_B2_MOTION:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            switch (code)
                            {
                                case CMD_WHEEL_B3_CONTROL:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    vehicle_set_motion(VEHICLE_MOTION_UNKNOWN);
                                    motor_set_direct(motor, MOTOR_DIRECTION_STOP);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_FREE:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    vehicle_set_motion(VEHICLE_MOTION_UNKNOWN);
                                    motor_set_direct(motor, MOTOR_DIRECTION_CLW);
                                    return RESULT_OK(NULL);
                                }
                                case CMD_WHEEL_B3_SLOW:
                                {
                                    vehicle_set_mode(VEHICLE_MODE_FREE);
                                    vehicle_set_motion(VEHICLE_MOTION_UNKNOWN);
                                    motor_set_direct(motor, MOTOR_DIRECTION_CCLW);
                                    return RESULT_OK(NULL);
                                }
                                default: break;
                            }
                            break;
                        }
                        case CMD_WHEEL_B2_SPEED:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            motor_set_rps_pcn(motor, code);
                            return RESULT_OK(NULL);
                        }
                        default: break;
                    }
                    break;
                }
                default: break;
            }
            break;
        }
        case CMD_VEHI_B0_CONTROL:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
            switch (code)
            {
                case CMD_VEHI_B1_MODE:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    switch (code)
                    {
                        case CMD_VEHI_B2_FREE:
                        {
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_END:
                        {
                            vehicle_set_mode(VEHICLE_MODE_END);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_F_TRACK:
                        {
                            vehicle_set_mode(VEHICLE_MODE_F_TRACK);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_TRACK:
                        {
                            vehicle_set_mode(VEHICLE_MODE_TRACK);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_SEARCH:
                        {
                            vehicle_set_mode(VEHICLE_MODE_SEARCH);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_ROTATE:
                        {
                            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
                            vehicle_set_need_rotate(code);
                            vehicle_set_mode(VEHICLE_MODE_ROTATE);
                            return RESULT_OK(NULL);
                        }
                        default: break;
                    }
                    break;
                }
                case CMD_VEHI_B1_MOTION:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    switch (code)
                    {
                        case CMD_VEHI_B2_FREE:
                        {
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            vehicle_set_motion(VEHICLE_MOTION_STOP);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_END:
                        {
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            vehicle_set_motion(VEHICLE_MOTION_FORWARD);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_F_TRACK:
                        {
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            vehicle_set_motion(VEHICLE_MOTION_BACKWARD);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_TRACK:
                        {
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            vehicle_set_motion(VEHICLE_MOTION_C_CLOCKWISE);
                            return RESULT_OK(NULL);
                        }
                        case CMD_VEHI_B2_SEARCH:
                        {
                            vehicle_set_mode(VEHICLE_MODE_FREE);
                            vehicle_set_motion(VEHICLE_MOTION_CLOCKWISE);
                            return RESULT_OK(NULL);
                        }
                        default: break;
                    }
                    break;
                }
                case CMD_VEHI_B1_SPEED:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    vehicle_set_mode(VEHICLE_MODE_FREE);
                    vehicle_set_speed(code);
                    return RESULT_OK(NULL);
                }
                default: break;
            }
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
}

static inline Result trsm_pkt_proc_inner(void)
{
    FdcanPkt *pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_pkt_write(pkt, DATA_TYPE_LEFT_SPEED);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_pkt_write(pkt, DATA_TYPE_LEFT_DUTY);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_pkt_write(pkt, DATA_TYPE_RIGHT_SPEED);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_pkt_write(pkt, DATA_TYPE_RIGHT_DUTY);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    return RESULT_OK(NULL);
}
#endif

#ifdef ANCILLARY_PROGRAM
#include "robotic_arm/main.h"
#include "rfid/main.h"
#include "map/main.h"

static inline Result fdcan_pkt_ist_read_inner(FdcanPkt* pkt, uint8_t byte0)
{
    uint8_t code;
    switch (byte0)
    {
        case CMD_MAP_B0_CONTROL:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
            switch (code)
            {
                // case CMD_MAP_B1_INFO:
                // {
                //     return simple_point_go();
                // }
                case CMD_MAP_B1_SET:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    switch (code)
                    {
                        case CMD_MAP_B2_FORWARD:
                        {
                            return map_update(SIMD_FOWARD);
                            // return simple_point_store(SIMD_FOWARD);
                        }
                        case CMD_MAP_B2_BACKWARD:
                        {
                            return map_update(SIMD_BACKWARD);
                            // return simple_point_store(SIMD_BACKWARD);
                        }
                        case CMD_MAP_B2_LEFT:
                        {
                            return map_update(SIMD_LEFT);
                            // return simple_point_store(SIMD_LEFT);
                        }
                        case CMD_MAP_B2_RIGHT:
                        {
                            return map_update(SIMD_RIGHT);
                            // return simple_point_store(SIMD_RIGHT);
                        }
                        default: break;
                    }
                    break;
                }
                default: break;
            }
            break;
        }
        case CMD_ARM_B0_CONTROL:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
            switch (code)
            {
                case CMD_ARM_B1_ARM:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    switch (code)
                    {
                        case CMD_ARM_B2_STOP:
                        {
                            arm_motion_set(ARM_MOTION_UNKNOWN);
                            arm_set_tim(&arm_bottom, arm_bottom.tim_current);
                            arm_set_tim(&arm_shoulder, arm_shoulder.tim_current);
                            arm_set_tim(&arm_elbow_btm, arm_elbow_btm.tim_current);
                            arm_set_tim(&arm_elbow_top, arm_elbow_top.tim_current);
                            arm_set_tim(&arm_wrist, arm_wrist.tim_current);
                            arm_set_tim(&arm_finger, arm_finger.tim_current);
                            return RESULT_OK(NULL);
                        }
                        default: break;
                    }
                    break;
                }
                case CMD_ARM_B1_BOTTOM:
                {
                    arm_motion_set(ARM_MOTION_UNKNOWN);
                    return arm_motor_set(pkt, &arm_bottom);
                }
                case CMD_ARM_B1_SHOULDER:
                {
                    arm_motion_set(ARM_MOTION_UNKNOWN);
                    return arm_motor_set(pkt, &arm_shoulder);
                }
                case CMD_ARM_B1_ELBOW_BTM:
                {
                    arm_motion_set(ARM_MOTION_UNKNOWN);
                    return arm_motor_set(pkt, &arm_elbow_btm);
                }
                case CMD_ARM_B1_ELBOW_TOP:
                {
                    arm_motion_set(ARM_MOTION_UNKNOWN);
                    return arm_motor_set(pkt, &arm_elbow_top);
                }
                case CMD_ARM_B1_WRIST:
                {
                    arm_motion_set(ARM_MOTION_UNKNOWN);
                    return arm_motor_set(pkt, &arm_wrist);
                }
                case CMD_ARM_B1_FINGER:
                {
                    arm_motion_set(ARM_MOTION_UNKNOWN);
                    return arm_motor_set(pkt, &arm_finger);
                }
                default: break;
            }
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
}

static inline Result fdcan_pkt_rcv_read_inner(FdcanPkt* pkt, uint8_t byte0)
{
    uint8_t code;
    switch (byte0)
    {
        case CMD_RFID_B0_CONTROL:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 1, &code));
            switch (code)
            {
                case CMD_RFID_B1_SELECT:
                {
                    uint8_t secter, block;
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &secter));
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &block));
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 4, &code));
                    switch (code)
                    {
                        case CMD_RFID_B4_ONLY_SET:
                        {
                            return rfid_trcv_buf_setaddr(&rfid_trsm_buf, secter, block, 0);
                        }
                        case CMD_RFID_B4_WRITE:
                        {
                            return rfid_trcv_buf_setaddr(&rfid_trsm_buf, secter, block, 1);
                        }
                        default: break;
                    }
                    break;
                }
                case CMD_RFID_B1_INP_DATA:
                {
                    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
                    if (pkt->len < 7) return RESULT_ERROR(RES_ERR_EMPTY);
                    return rfid_trcv_buf_setdata(&rfid_trsm_buf, code * 4, pkt->data + 3, 4);
                }
                default: break;
            }
            break;
        }
        default: return RESULT_ERROR(RES_ERR_NOT_FOUND);
    }
}

static inline Result trsm_pkt_proc_inner(void)
{
    FdcanPkt *pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_pkt_write(pkt, DATA_TYPE_ARM_BOTTOM));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_pkt_write(pkt, DATA_TYPE_ARM_SHOULDER));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_pkt_write(pkt, DATA_TYPE_ARM_ELBOW_BTM));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_pkt_write(pkt, DATA_TYPE_ARM_ELBOW_TOP));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_pkt_write(pkt, DATA_TYPE_ARM_WRIST));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_pkt_write(pkt, DATA_TYPE_ARM_FINGER));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    return RESULT_OK(NULL);
}

static Result arm_motor_set(FdcanPkt* pkt, ArmMotorParameter* arm)
{
    uint8_t code;
    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 2, &code));
    switch (code)
    {
        case CMD_ARM_B2_STOP:
        {
            arm_set_tim(arm, arm->tim_current);
            return RESULT_OK(NULL);
        }
        case CMD_ARM_B2_SET:
        {
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 3, &code));
            arm_set_pos(arm, code);
            return RESULT_OK(NULL);
        }
        default: break;
    }
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}
#endif

#ifdef MCU_MOTOR_CTRL
#include "motor/basic.h"

Result fdcan_pkt_ist_read(FdcanPkt* pkt)
{
    uint8_t code;
    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
    switch (code)
    {
        case CMD_DATA_B0_STOP:
        {
            fdacn_data_store = FNC_DISABLE;
            return RESULT_OK(NULL);
        }
        case CMD_DATA_B0_START:
        {
            fdacn_data_store = FNC_ENABLE;
            return RESULT_OK(NULL);
        }
        case CMD_WHEEL_B0_CONTROL:
        {
            #define SPD_START_BYTE 4
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, SPD_START_BYTE + 2, &code));
            bool reverse = (!code) ? 0 : 1;
            RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, SPD_START_BYTE + 3, NULL));
            uint8_t spd_u8[4];
            memcpy(spd_u8, pkt->data + SPD_START_BYTE, 4);
            float32_t spd_f32 = var_u8_to_f32_be(spd_u8);
            motor_set_speed(&motor_h, reverse, spd_f32);
            return RESULT_OK(NULL);
        }
        default: return fdcan_pkt_ist_read_inner(pkt);
    }
}
#endif

Result fdcan_pkt_rcv_read(FdcanPkt* pkt)
{
    uint8_t code;
    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
    switch (code)
    {
        default: return fdcan_pkt_rcv_read_inner(pkt);
    }
}
