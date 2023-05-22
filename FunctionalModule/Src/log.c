#include "log.h"

#include "cmsis_os.h"
#include "queue.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// log
static TurtleLog *g_pTurtleLog = NULL;
// size_t            sizeNow;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == g_pTurtleLog->m_pHuart)
    {
        if (g_pTurtleLog == NULL)
        {
            return;
        }
        HAL_UART_DMAStop(g_pTurtleLog->m_pHuart);

        // sizeNow = osSemaphoreGetCount(g_pTurtleLog->m_pSendStatusSemaphore);
        // osSemaphoreGetCount(g_pTurtleLog->m_pSendStatusSemaphore);
        osSemaphoreRelease(g_pTurtleLog->m_pSendStatusSemaphore);
    }
}

/**
 * @brief Task For Log Framwok
 *
 * @param arg
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
void LogTask(void *arg)
{
    LogMessage pLogData;

    for (;;)
    {
        if (g_pTurtleLog == NULL)
        {
            continue;
        }
        if (osSemaphoreAcquire(g_pTurtleLog->m_pSendStatusSemaphore, 100) == osOK)
        {
            if (osMessageQueueGet(g_pTurtleLog->m_pMessageQueue, &pLogData, NULL, 100) == osOK)
            {
                // BUG Wait Fix

                // BUG描述 osMessageQueueGet将会出现
                // 临时方案
                if (osMessageQueueGetCount(g_pTurtleLog->m_pMessageQueue) == 0)
                {
                    osMessageQueueReset(g_pTurtleLog->m_pMessageQueue);
                }

                if (g_pTurtleLog->m_pPrevMessage != NULL)
                {
                    vPortFree(g_pTurtleLog->m_pPrevMessage);
                }

                g_pTurtleLog->m_pPrevMessage = pLogData.m_pMessage;
                // HAL_UART_Transmit(g_pTurtleLog->m_pHuart, (unsigned char *)pLogData.m_pMessage,
                //                       pLogData.messageLength, 500);
                HAL_UART_Transmit_DMA(g_pTurtleLog->m_pHuart, (unsigned char *)pLogData.m_pMessage,
                                      pLogData.messageLength);
            }
            else
            {
                osSemaphoreRelease(g_pTurtleLog->m_pSendStatusSemaphore);
            }
        }
        osDelay(10);
    }
}

/**
 * @brief Write Log To Serial
 *
 * @param pLog
 * @param level
 * @param pTag
 * @param pMessage
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
void SerialLog(LogLevel level, const char *pTag, const char *pMessage, ...)
{
    // TODO 优化该函数，使用队列来缓存消息
    // !important 优化该函数在栈上的开销
    char *pTempData = pvPortMalloc(64);

    memset(pTempData, 0, 64);
    char pMessageTemp[40];

    // 处理可变参数
    va_list args;
    va_start(args, pMessage);
    vsnprintf(pMessageTemp, 40 + 1, pMessage, args);
    va_end(args);

    // 日志级别设置
    const char *logLevel = NULL;
    if (level == LOG_INFO)
    {
        logLevel = "INFO";
    }
    else if (level == LOG_WARNING)
    {
        logLevel = "WARN";
    }
    else if (level == LOG_DEBUG)
    {
        logLevel = "DBUG";
    }
    else if (level == LOG_ERROR)
    {
        logLevel = "ERRO";
    }
    else
    {
        logLevel = "UNKN";
    }

    // 写入格式化日志
    sprintf(pTempData, "%s [%s]: %s\r\n", logLevel, pTag, pMessageTemp);

    LogMessage logData;
    logData.m_pMessage    = pTempData;
    logData.messageLength = 64;

    // 加入队列
    if (osMessageQueuePut(g_pTurtleLog->m_pMessageQueue, &logData, 0, 100) != osOK)
    {
        vPortFree(pTempData);
    }
}

void InitializeLog(UART_HandleTypeDef *huart)
{
    // has initialized
    if (g_pTurtleLog != NULL)
    {
        return;
    }

    g_pTurtleLog           = (TurtleLog *)pvPortMalloc(sizeof(TurtleLog));
    g_pTurtleLog->m_pHuart = huart;

    const osMessageQueueAttr_t logQueueHandle_attributes = {.name = "logQueue"};

    g_pTurtleLog->m_pMessageQueue = osMessageQueueNew(16, sizeof(LogMessage), &logQueueHandle_attributes);

    const osThreadAttr_t logTask_attributes = {
        .name       = "logTask",
        .stack_size = 128,
        .priority   = (osPriority_t)osPriorityNormal,
    };

    // Start Log Task
    g_pTurtleLog->m_pLogThread = osThreadNew(LogTask, NULL, &logTask_attributes);

    g_pTurtleLog->m_pSendStatusSemaphore = osSemaphoreNew(1, 1, NULL);

    if (g_pTurtleLog->m_pMessageQueue == NULL)
    {
        HAL_UART_Transmit(huart, (unsigned char *)"ErrInitQueue\r\n", 14, 500);
    }

    if (g_pTurtleLog->m_pLogThread == NULL)
    {
        HAL_UART_Transmit(huart, (unsigned char *)"ErrInitThread\r\n", 15, 500);
    }

    if (g_pTurtleLog->m_pSendStatusSemaphore == NULL)
    {
        HAL_UART_Transmit(huart, (unsigned char *)"ErrInitStatus\r\n", 15, 500);
    }
}
