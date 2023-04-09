/**
 * @file lsm6dos.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TURTLE_FLIGHT_LSM6DOS_H
#define TURTLE_FLIGHT_LSM6DOS_H

#include "common.h"
#include <stdint.h>

/*******  Read Write Address */
#define READ_ADDRESS 0xD5
#define WRITE_ADDRESS 0xD4

#define WHO_AM_I 0x0FU
#define WHO_AM_I_VALUE 0x6C

// Read Status
#define STATUS_REG 0x1EU

#define TEMPERATURE_NEWDATA_AVAILABLE (0x01 << 2)
#define STATUS_GYROSCOPE_NEWDATA_AVAILABLE (0x01 << 1)
#define STATUS_ACCELEROMETER_NEWDATA_AVAILABLE (0x01)

// INT1 Control Reg
#define INT1_CTRL 0x0DU

#define WORK_26HZ_MASK (0x10)   // 0001 0000
#define WORK_208HZ_MASK (0x50)  // 0101 0000
#define WORK_6660HZ_MASK (0xA0) // 1010 0000
#define POWER_DOWN 0x00

// ACC Work Mode
#define CTRL1_XL 0x10U
// GYRO Work Mode
#define CTRL2_G 0x11U
// Control Register 3
#define CTRL3_C 0x12U

// ResetMask
#define RESET_MASK 0x01

#define OUT_TEMP_L 0x20U
#define OUT_TEMP_H 0x21U

/************* XYZ */

// angular rate sensor pitch axis angular rate output register
#define OUTX_L_G 0x22U
#define OUTX_H_G 0x23U

// angular rate sensor roll axis angular rate output register
#define OUTY_L_G 0x24U
#define OUTY_H_G 0x25U

// angular rate sensor yaw axis angular rate output register
#define OUTZ_L_G 0x26U
#define OUTZ_H_G 0x27U

// linear acceleration sensor X-axis output register
#define OUTX_L_A 0x28U
#define OUTX_H_A 0x29U

// linear acceleration sensor Y-axis output register
#define OUTY_L_A 0x2AU
#define OUTY_H_A 0x2BU

// linear acceleration sensor Z-axis output register
#define OUTZ_L_A 0x2CU
#define OUTZ_H_A 0x2DU

typedef enum
{
    LSM6DOS_WorkStatusFullPower   = 0,
    LSM6DOS_WorkStatusLowPower    = 1,
    LSM6DOS_WorkStatusMiddlePower = 2,
    LSM6DOS_WorkStatusShutDown    = 3,

    LSM6DOS_WorkStatus_END = 0xFF
} LSM6DOS_WORK_STATUS;

typedef enum
{
    LSM6DOS_DeviceStatusPowerDown  = 0,
    LSM6DOS_DeviceStatusWork       = 1,
    LSM6DOS_DeviceStatusNotAllowed = 2,

    LSM6DOS_DeiceStatus_END = 0xFF
} LSM6DOS_DEVICE_STATUS;

/**
 * @brief gyro data
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-19
 * @copyright Copyright (c) 2023
 */
typedef struct
{
    int16_t pitch;
    int16_t roll;
    int16_t yaw;
} GYRO_DataStructure;

/**
 * @brief acc data
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-19
 * @copyright Copyright (c) 2023
 */
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} ACC_DataStructure;

typedef struct
{
    float x;
    float y;
    float z;
} AngleSructure;

/**
 * @brief Return Data Status For LSM6DOS
 *
 * @return LSM6DOS_STATUS
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-19
 * @copyright Copyright (c) 2023
 */
uint8_t LSM6DOS_GetDataStatus();

/**
 * @brief Device Status For LSM6DOS
 *
 * @return LSM6DOS_DEVICE_STATUS
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-19
 * @copyright Copyright (c) 2023
 */
LSM6DOS_DEVICE_STATUS LSM6DOS_GetDeviceStatus();

/**
 * @brief Reset Device
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-19
 * @copyright Copyright (c) 2023
 */
void LSM6DOS_Reset();

/**
 * @brief Work Status
 *
 * @return LSM6DOS_DEVICE_STATUS
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-19
 * @copyright Copyright (c) 2023
 */
LSM6DOS_DEVICE_STATUS LSM6DOS_SetWorkStatus(LSM6DOS_WORK_STATUS workStatus);

DriverStatus LSM6DOS_Initialize();

int16_t LSM6DOS_GetTemperature();

void LSM6DOS_GetGyro(GYRO_DataStructure *gyro);
void LSM6DOS_GetAcc(ACC_DataStructure *acc);

#endif // TURTLE_FLIGHT_LSM6DOS_H
