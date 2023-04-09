/**
 * @file test.c
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief Test For Module data
 * @version 0.1
 * @date 2023-03-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "test.h"

#include "bmp280.h"
#include "log.h"
#include "lsm6dos.h"
#include "main.h"
#include "status_light.h"
#include "w25q16.h"
#include "ws2812.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

osThreadId_t pTestTaskId;

float ComplementaryFilter(float acc, float gyro, float dt, float lastAngle)
{
    float a     = 0.96;
    float angle = a * (lastAngle + gyro * dt) + (1 - a) * (acc);
    return angle;
}

AngleSructure angleData;

void DataCalcTimer(void *)
{
    // Sync
    GYRO_DataStructure gyro;
    ACC_DataStructure  acc;

    if ((LSM6DOS_GetDataStatus() & (STATUS_GYROSCOPE_NEWDATA_AVAILABLE & STATUS_ACCELEROMETER_NEWDATA_AVAILABLE)) ==
        (STATUS_GYROSCOPE_NEWDATA_AVAILABLE & STATUS_ACCELEROMETER_NEWDATA_AVAILABLE))
    {
        LSM6DOS_GetGyro(&gyro);
        LSM6DOS_GetAcc(&acc);

        // 此时采样的是 2g
        float x = acc.x / 2000.0f;
        float y = acc.y / 2000.0f;
        float z = acc.z / 2000.0f;

        // 125分辨率
        float pi = gyro.pitch * 4.375f / 1000.0;
        float ro = gyro.roll * 4.375f / 1000.0;
        float ya = gyro.yaw * 4.375f / 1000.0;

        // 转换为角度
        float accAngleX = atan2(y, z) * 180 / 3.14;
        float accAngleY = atan2(z, x) * 180 / 3.14;
        float accAngleZ = atan2(x, y) * 180 / 3.14;

        angleData.x = ComplementaryFilter(accAngleX, pi, 0.005, angleData.x);
        angleData.y = ComplementaryFilter(accAngleY, ro, 0.005, angleData.y);
        angleData.z = ComplementaryFilter(accAngleZ, ya, 0.005, angleData.z);
    }
}

void InitTest()
{
#if defined(TEST_ALL) || defined(TEST_LOG)

    InitializeLog(&huart1);

#endif

#if defined(TEST_ALL) || defined(TEST_WS2812)

    WS2812_Initialize();

#endif

#if defined(TEST_ALL) || defined(TEST_BMP280)

    if (BMP280_Initialize() == DRIVER_OK)
    {
        SerialLog(LOG_INFO, "TEST", "Initialize BARO Successful!");
    }
    else
    {
        SerialLog(LOG_ERROR, "TEST", "Error For Initialize BARO!");
    }

#endif

#if defined(TEST_ALL) || defined(TEST_W25Q16)

    if (W25Q16_Initialize() == DRIVER_OK)
    {
        SerialLog(LOG_INFO, "TEST", "Initialize FLASH Successful!");
    }
    else
    {
        SerialLog(LOG_ERROR, "TEST", "Error For Initialize FLASH!");
    }

#endif

#if defined(TEST_ALL) || defined(TEST_LSM6DOS)

    if (LSM6DOS_Initialize() == DRIVER_OK)
    {
        SerialLog(LOG_INFO, "TEST", "Initialize ACC Successful!");
    }
    else
    {
        SerialLog(LOG_ERROR, "TEST", "Error For Initialize ACC!");
    }

#endif

    const osThreadAttr_t testTask_attributes = {
        .name       = "testTask",
        .stack_size = 64 * 8,
        .priority   = (osPriority_t)osPriorityLow,
    };

    pTestTaskId = osThreadNew(TestTask, NULL, &testTask_attributes);

    osTimerAttr_t dataTimerAttr = {.name = "dataTimer"};

    osTimerId_t timer = osTimerNew(&DataCalcTimer, osTimerPeriodic, NULL, &dataTimerAttr);
    // 5ms once
    osTimerStart(timer, 5);
}

void TestTask(void *tes)
{
    for (;;)
    {
#if defined(TEST_ALL) || defined(TEST_LOG)
        // SerialLog(LOG_WARNING, "LOG", "TEST");
        // osDelay(100);
#endif

#if defined(TEST_ALL) || defined(TEST_WS2812)
        WS2812_Write(0xFF >> 5, 0xFF >> 5, 0xFF >> 5);
        osDelay(1000);
#endif

#if defined(TEST_ALL) || defined(TEST_BMP280)
        // SerialLog(LOG_INFO, "BMP280", "I 0x%x", BMP280_GetId());
#endif

#if defined(TEST_ALL) || defined(TEST_W25Q16)
        // W25Q16_GetId();
#endif

#if defined(TEST_ALL) || defined(TEST_LSM6DOS)
        if (LSM6DOS_SetWorkStatus(LSM6DOS_WorkStatusFullPower) != LSM6DOS_DeviceStatusWork)
        {
            SerialLog(LOG_WARNING, "LSM6DOS", "Error");
        }

        while (1)
        {
            SerialLog(LOG_INFO, "TEST", "angle x:%d y:%d z:%d", (int)(angleData.x), (int)(angleData.y),
                      (int)(angleData.z));
            // char data[100];
            // memset(data, 0, 100);
            // sprintf(data, "angle x:%d y:%d z:%d \r\n", (int)(angleData.x), (int)(angleData.y), (int)(angleData.z));
            // HAL_UART_Transmit(&huart1, data, 100, 1000);
            osDelay(100);
        }
#endif
        // Release Test Thread
        osThreadExit();
    }
}
