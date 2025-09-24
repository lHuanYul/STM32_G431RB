#include "connectivity/fdcan/main.h"
#include "fdcan.h"
#include "connectivity/cmds.h"
#include "connectivity/fdcan/pkt_write.h"

__weak Result instant_recv_proc_inner(FdcanPkt* pkt, uint8_t byte0)
{
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}

__weak Result recv_pkt_proc_inner(FdcanPkt* pkt, uint8_t byte0)
{
    return RESULT_ERROR(RES_ERR_NOT_FOUND);
}

__weak Result trsm_pkt_proc_inner(void)
{
    return RESULT_OK(NULL);
}

#ifdef PRINCIPAL_PROGRAM
#include "vehicle/basic.h"
#include "motor/main.h"

static inline Result instant_recv_proc_inner(FdcanPkt* pkt, uint8_t byte0)
{
    uint8_t code;
    switch (byte0)
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
    fdcan_data_pkt_write(pkt, DATA_TYPE_LEFT_SPEED);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_data_pkt_write(pkt, DATA_TYPE_LEFT_DUTY);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_data_pkt_write(pkt, DATA_TYPE_RIGHT_SPEED);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    fdcan_data_pkt_write(pkt, DATA_TYPE_RIGHT_DUTY);
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    return RESULT_OK(NULL);
}
#endif

#ifdef ANCILLARY_PROGRAM
#include "robotic_arm/main.h"
#include "rfid/main.h"
#include "map/main.h"

static inline Result instant_recv_proc_inner(FdcanPkt* pkt, uint8_t byte0)
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

static inline Result recv_pkt_proc_inner(FdcanPkt* pkt, uint8_t byte0)
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
    RESULT_CHECK_HANDLE(fdcan_data_pkt_write(pkt, DATA_TYPE_ARM_BOTTOM));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_data_pkt_write(pkt, DATA_TYPE_ARM_SHOULDER));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_data_pkt_write(pkt, DATA_TYPE_ARM_ELBOW_BTM));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_data_pkt_write(pkt, DATA_TYPE_ARM_ELBOW_TOP));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_data_pkt_write(pkt, DATA_TYPE_ARM_WRIST));
    fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
    pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
    RESULT_CHECK_HANDLE(fdcan_data_pkt_write(pkt, DATA_TYPE_ARM_FINGER));
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

Result instant_recv_proc(FdcanPkt* pkt)
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
        default: return instant_recv_proc_inner(pkt, code);
    }
}

static FDCAN_TxHeaderTypeDef TxHeader = {
    .ErrorStateIndicator = FDCAN_ESI_PASSIVE,
    .TxEventFifoControl = FDCAN_STORE_TX_EVENTS,
};
// static FdcanPkt fdcan_trsm_pkt = {0};
static UNUSED_FNC void pkt_transmit(void)
{
    Result result = fdcan_pkt_buf_pop(&fdcan_trsm_pkt_buf);
    if (RESULT_CHECK_RAW(result)) return;
    FdcanPkt* pkt = RESULT_UNWRAP_HANDLE(result);
    // memcpy(pkt, &fdcan_trsm_pkt, sizeof(FdcanPkt));
    TxHeader.Identifier = pkt->id;
    TxHeader.DataLength = pkt->len;
    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, pkt->data);
    fdcan_pkt_pool_free(pkt);
}

static UNUSED_FNC Result trsm_pkt_proc(void)
{
    Result result = RESULT_OK(NULL);
    if (fdacn_data_store == FNC_ENABLE)
    {
        #ifdef ENABLE_CON_PKT_TEST
        FdcanPkt *pkt = RESULT_UNWRAP_HANDLE(fdcan_pkt_pool_alloc());
        fdcan_data_pkt_write(pkt, DATA_TYPE_TEST);
        fdcan_pkt_buf_push(&fdcan_trsm_pkt_buf, pkt);
        #else
        return trsm_pkt_proc_inner();
        #endif
    }
    return result;
}

static Result recv_pkt_proc_e(FdcanPkt* pkt)
{
    uint8_t code;
    RESULT_CHECK_RET_RES(fdcan_pkt_get_byte(pkt, 0, &code));
    switch (code)
    {
        default: return recv_pkt_proc_inner(pkt, code);
    }
}

static UNUSED_FNC Result recv_pkt_proc(size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        FdcanPkt* pkt = RESULT_UNWRAP_RET_RES(fdcan_pkt_buf_pop(&fdcan_recv_pkt_buf));
        recv_pkt_proc_e(pkt);
        fdcan_pkt_pool_free(pkt);
    }
    return RESULT_OK(NULL);
}

#define FDCAN_TASK_DELAY_MS 10
void StartFdCanTask(void *argument)
{
    #ifdef DISABLE_FDCAN
    StopTask();
    #else
    fdcan_pkt_pool_init();
    ERROR_CHECK_HAL_HANDLE(HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE));
    FDCAN_FilterTypeDef sFilter0 = {
        .IdType = FDCAN_STANDARD_ID,
        .FilterIndex = 0,
        .FilterType = FDCAN_FILTER_RANGE,
        .FilterConfig = FDCAN_FILTER_TO_RXFIFO0_HP,
        .FilterID1 = FDCAN_FILTER0_ID_MIN,
        .FilterID2 = FDCAN_FILTER0_ID_MAX,
    };
    ERROR_CHECK_HAL_HANDLE(HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilter0));
    FDCAN_FilterTypeDef sFilter1 = {
        .IdType = FDCAN_STANDARD_ID,
        .FilterIndex = 1,
        .FilterType = FDCAN_FILTER_RANGE,
        .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
        .FilterID1 = FDCAN_FILTER1_ID_MIN,
        .FilterID2 = FDCAN_FILTER1_ID_MAX,
    };
    ERROR_CHECK_HAL_HANDLE(HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilter1));
    ERROR_CHECK_HAL_HANDLE(HAL_FDCAN_Start(&hfdcan1));
    ERROR_CHECK_HAL_HANDLE(
        HAL_FDCAN_ActivateNotification(&hfdcan1,
              FDCAN_IT_BUS_OFF
            | FDCAN_IT_TX_EVT_FIFO_NEW_DATA
            | FDCAN_IT_TX_EVT_FIFO_FULL
            | FDCAN_IT_TX_EVT_FIFO_ELT_LOST
        , 0)
    );
    ERROR_CHECK_HAL_HANDLE(
        HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_TX_COMPLETE,
              FDCAN_TX_BUFFER0
            | FDCAN_TX_BUFFER1
            | FDCAN_TX_BUFFER2
        )
    );
    ERROR_CHECK_HAL_HANDLE(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0));
    ERROR_CHECK_HAL_HANDLE(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0));
    size_t tick = 0;
    const uint32_t osPeriod = pdMS_TO_TICKS(FDCAN_TASK_DELAY_MS);
    uint32_t next_wake = osKernelGetTickCount() + osPeriod;
    for(;;)
    {
        if (fdcan_bus_off)
        {
            fdcan_bus_off = false;
            HAL_FDCAN_Stop(&hfdcan1);
            HAL_FDCAN_Start(&hfdcan1);
        }
        pkt_transmit();
        recv_pkt_proc(5);
        if (tick % 50 == 0)
        {
            tick = 0;
            trsm_pkt_proc();
        }
        osDelayUntil(next_wake);
        next_wake += osPeriod;
        tick++;
    }
    #endif
}
