#include "connectivity/fdcan/main.h"
#include "fdcan.h"
#include "connectivity/cmds.h"
#include "connectivity/fdcan/pkt_read.h"
#include "connectivity/fdcan/pkt_write.h"

static FDCAN_TxHeaderTypeDef TxHeader = {
    .ErrorStateIndicator = FDCAN_ESI_PASSIVE,
    .TxEventFifoControl = FDCAN_STORE_TX_EVENTS,
};

static void pkt_transmit(void)
{
    Result result = fdcan_pkt_buf_pop(&fdcan_trsm_pkt_buf);
    if (RESULT_CHECK_RAW(result)) return;
    FdcanPkt* pkt = RESULT_UNWRAP_HANDLE(result);
    TxHeader.Identifier = pkt->id;
    TxHeader.DataLength = pkt->len;
    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, pkt->data);
    fdcan_pkt_pool_free(pkt);
}

static Result recv_pkts_proc(size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        FdcanPkt* pkt = RESULT_UNWRAP_RET_RES(fdcan_pkt_buf_pop(&fdcan_recv_pkt_buf));
        fdcan_pkt_rcv_read(pkt);
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
        recv_pkts_proc(5);
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
