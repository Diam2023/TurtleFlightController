#include "bmp280.h"

#include "common.h"
#include "main.h"
#include "spi.h"

#define BUFFER_SIZE 32
#define CS_ON HAL_GPIO_WritePin(BMP280_CS_GPIO_Port, BMP280_CS_Pin, GPIO_PIN_RESET)
#define CS_OFF HAL_GPIO_WritePin(BMP280_CS_GPIO_Port, BMP280_CS_Pin, GPIO_PIN_SET)

static uint8_t g_pSendDataBuffer[BUFFER_SIZE];
static uint8_t g_pReceiveDataBuffer[BUFFER_SIZE];

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1)
    {
    }
}
static void ResetBuffer()
{
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        g_pSendDataBuffer[i]    = 0;
        g_pReceiveDataBuffer[i] = 0;
    }
}

uint8_t BMP280_GetId()
{
    CS_OFF;
    ResetBuffer();
    g_pSendDataBuffer[0] = 0xD0;
    CS_ON;
    HAL_SPI_TransmitReceive(&hspi1, g_pSendDataBuffer, g_pReceiveDataBuffer, 2, 100);
    CS_OFF;
    return g_pReceiveDataBuffer[1];
}

/**
 * @brief Check BMP280 ID Is true
 *
 * @return true It Is BMP280
 * @return false It Is Not BMP280
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-16
 * @copyright Copyright (c) 2023
 */
bool BMP280_Check()
{
    // Enable
    CS_ON;
    ResetBuffer();
    g_pSendDataBuffer[0] = 0xD0;
    HAL_SPI_TransmitReceive(&hspi1, g_pSendDataBuffer, g_pReceiveDataBuffer, sizeof(uint8_t) * 2, 1000);

    bool result = false;
    if (g_pReceiveDataBuffer[1] == 0x58)
    {
        result = true;
    }

    // Disable
    CS_OFF;
    return result;
}

/**
 * @brief Reset BMP280
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
void BMP280_Reset()
{
    // Enable
    CS_ON;
    // RESET
    ResetBuffer();
    g_pSendDataBuffer[0] = 0xE0;
    g_pSendDataBuffer[1] = 0xB6;
    HAL_SPI_TransmitReceive(&hspi1, g_pSendDataBuffer, g_pReceiveDataBuffer, sizeof(uint8_t) * 2, 100);

    // Disable
    CS_OFF;
}

/**
 * @brief BMP280 Initialize
 *
 * @return DriverStatus
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-16
 * @copyright Copyright (c) 2023
 */
DriverStatus BMP280_Initialize()
{
    CS_OFF;
    // Get ID
    if (BMP280_Check() == false)
    {
        // Id Cannot found
        return DRIVER_NOTFOUND;
    }

    CS_OFF;
    // Driver Initialize Ok
    return DRIVER_OK;
}
