/*
#include "main/fn_state.h"
#include "fn_state.h"
*/
#pragma once

#include "main/config.h"

typedef struct SuccessResult
{
    void *obj;
} SuccessResult;

typedef enum ErrorType
{
    RES_ERR_INVALID = -1,
    RES_ERR_UND     = 0,
    RES_ERR_FAIL    = 1,
    RES_ERR_BUSY    = 2,
    RES_ERR_TIMEOUT = 3,
    RES_ERR_MEMORY_ERROR,
    RES_ERR_EMPTY,
    RES_ERR_FULL,
    RES_ERR_OVERFLOW,
    RES_ERR_NOT_FOUND,
    RES_ERR_NOT_MOVE,
    RES_ERR_REMOVE_FAIL,
    RES_ERR_DIV_0,
} ErrorType;

extern ErrorType last_error;

typedef struct Result
{
    bool is_ok;
    union
    {
        SuccessResult success;
        ErrorType error;
    } result;
} Result;

// RESULT_OK(NULL);
#define RESULT_OK(_obj_) ((Result){.is_ok = true, .result.success = {.obj = (_obj_)}})

#define RESULT_ERROR(_err_) ((Result){.is_ok = false, .result.error = (_err_)})

#define RESULT_BOOL(_cond_) ((_cond_) ? RESULT_OK(NULL) : RESULT_ERROR(RES_ERR_FAIL))

#define RESULT_CHECK_RAW(res) (!(res).is_ok)

#define RESULT_CHECK_SIMPLE(res)    \
    do {                            \
        if (RESULT_CHECK_RAW(res))  \
            return EXIT_FAILURE;    \
    } while (0)

    
#define RESULT_CHECK_HANDLE(expr)           \
    do {                                    \
        Result res = (expr);                \
        if (RESULT_CHECK_RAW(res))          \
        {                                   \
            last_error = res.result.error;  \
            Error_Handler();                \
        }                                   \
    } while (0)

#define RESULT_UNWRAP_HANDLE(expr)          \
    ({                                      \
        Result res = (expr);                \
        if (RESULT_CHECK_RAW(res))          \
        {                                   \
            last_error = res.result.error;  \
            Error_Handler();                \
        }                                   \
        (res).result.success.obj;           \
    })

#define RESULT_CHECK_RET_VOID(expr)         \
    do {                                    \
        Result res = (expr);                \
        if (RESULT_CHECK_RAW(res))          \
        {                                   \
            last_error = res.result.error;  \
            return;                         \
        }                                   \
    } while (0)

#define RESULT_UNWRAP_RET_VOID(res) \
    ({                              \
        RESULT_CHECK_RET_VOID(res); \
        (res).result.success.obj;   \
    })

#define RESULT_CHECK_RET_RES(expr)          \
    do {                                    \
        Result res = (expr);                \
        if (RESULT_CHECK_RAW(res))          \
        {                                   \
            last_error = res.result.error;  \
            return res;                     \
        }                                   \
    } while (0)

#define RESULT_UNWRAP_RET_RES(res)  \
    ({                              \
        RESULT_CHECK_RET_RES(res);  \
        (res).result.success.obj;   \
    })

#define RESULT_CHECK_CLEANUP(expr)              \
    do {                                        \
        result = (expr);                        \
        if (RESULT_CHECK_RAW(result))           \
        {                                       \
            last_error = result.result.error;   \
            goto cleanup;                       \
        }                                       \
    } while (0)

#ifdef STM32_DEVICE
#define ERROR_CHECK_HAL_RET_HAL(expr)       \
    do {                                    \
        HAL_StatusTypeDef _err = (expr);    \
        if (_err != HAL_OK)                 \
        {                                   \
            return _err;                    \
        }                                   \
    } while (0)

#define ERROR_CHECK_HAL_RET_RES(expr)       \
    do {                                    \
        HAL_StatusTypeDef _err = (expr);    \
        if (_err != HAL_OK)                 \
        {                                   \
            return RESULT_ERROR(_err);      \
        }                                   \
    } while (0)

#define ERROR_CHECK_HAL_HANDLE(expr)        \
    do {                                    \
        HAL_StatusTypeDef _err = (expr);    \
        if (_err != HAL_OK)                 \
        {                                   \
            last_error = _err;              \
            Error_Handler();                \
        }                                   \
    } while (0)
#endif

#define StopTask()  \
({                  \
    osThreadExit(); \
    return;         \
})
