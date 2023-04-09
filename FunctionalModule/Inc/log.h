/**
 * @file log.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TURTLE_FLIGHT_LOG_H
#define TURTLE_FLIGHT_LOG_H

#include "usart.h"
#include "common.h"
#include "cmsis_os2.h"

/**
 * @brief Log level
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
typedef enum
{
    LOG_INFO    = 0,
    LOG_DEBUG   = 1,
    LOG_WARNING = 2,
    LOG_ERROR   = 3
} LogLevel;

/**
 * @brief 
 * 
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
#pragma pack(1)
typedef struct
{
    public
    // pointer for log output device
    UART_HandleTypeDef *m_pHuart;

    // last send message, use for release
    char* m_pPrevMessage;

    osMessageQueueId_t m_pMessageQueue;

    // send status
    // osMutexId_t m_pSendStatusMutex;
    osSemaphoreId_t m_pSendStatusSemaphore;

    osThreadId_t m_pLogThread;
} TurtleLog;

#pragma pack(1)
typedef struct
{
    public
    size_t messageLength;
    char* m_pMessage;
} LogMessage;

/**
 * @brief Initialize
 *
 * @param huart
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
void InitializeLog(UART_HandleTypeDef *huart);

void SerialLog(LogLevel level, const char *pTag, const char *pMessage, ...);

#endif // TURTLE_FLIGHT_LOG_H