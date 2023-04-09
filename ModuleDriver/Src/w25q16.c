/**
 * @file w25q16.c
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-16
 *
 * @copyright Copyright (c) 2023
 * @link CSDN https://blog.csdn.net/weixin_43482414/article/details/103979159
 */

#include "w25q16.h"
#include "main.h"
#include "spi.h"

#define BUFFER_SIZE 32
#define CS_ON HAL_GPIO_WritePin(W25Q16_CS_GPIO_Port, W25Q16_CS_Pin, GPIO_PIN_RESET)
#define CS_OFF HAL_GPIO_WritePin(W25Q16_CS_GPIO_Port, W25Q16_CS_Pin, GPIO_PIN_SET)

static uint8_t g_pSendDataBuffer[BUFFER_SIZE];
static uint8_t g_pReceiveDataBuffer[BUFFER_SIZE];

static void ResetBuffer()
{
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        g_pSendDataBuffer[i]    = 0;
        g_pReceiveDataBuffer[i] = 0;
    }
}

uint16_t W25Q16_GetId()
{
    CS_OFF;
    ResetBuffer();
    g_pSendDataBuffer[0] = 0x90;
    CS_ON;

    HAL_SPI_Transmit(&hspi1, g_pSendDataBuffer, 4, 100);
    ResetBuffer();
    HAL_SPI_TransmitReceive(&hspi1, g_pSendDataBuffer, g_pReceiveDataBuffer, 2, 100);
    CS_OFF;

    uint16_t result = ((g_pReceiveDataBuffer[1] << 8) | (g_pReceiveDataBuffer[0]));
    return result;
}

void W25Q16_Reset()
{
    g_pSendDataBuffer[0] = RESET_ENABLE_CMD;
    g_pSendDataBuffer[1] = RESET_MEMORY_CMD;

    CS_ON;
    /* Send the reset command */
    HAL_SPI_Transmit(&hspi1, g_pSendDataBuffer, 2, 100);
    CS_OFF;
}

bool W25Q16_Check()
{
    if (W25Q16_GetId() == 0x14EF)
    {
        return true;
    }
    return false;
}

/**
 * @brief  Reads current status of the W25Q128FV.
 *
 * @retval W25Q128FV memory status
 */
static uint8_t W25Q16_GetStatus()
{
    g_pSendDataBuffer[0] = READ_STATUS_REG1_CMD;
    uint8_t status;

    CS_ON;
    /* Send the read status command */
    HAL_SPI_Transmit(&hspi1, g_pReceiveDataBuffer, 1, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    HAL_SPI_Receive(&hspi1, &status, 1, W25Qx_TIMEOUT_VALUE);
    CS_OFF;

    /* Check the value of the register */
    if ((status & W25Q128FV_FSR_BUSY) != 0)
    {
        return DRIVER_BUSY;
    }
    return DRIVER_OK;
}

DriverStatus W25Q16_Initialize()
{
    if (W25Q16_Check() == true)
    {
        W25Q16_Reset();

        return W25Q16_GetStatus();
    }
    else
    {
        return DRIVER_NOTFOUND;
    }
}
