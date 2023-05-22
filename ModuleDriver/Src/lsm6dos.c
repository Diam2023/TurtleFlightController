/**
 * @file lsm6dos.c
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "lsm6dos.h"

#include "i2c.h"
#include "main.h"

DriverStatus LSM6DOS_Initialize()
{
    uint8_t id = 0;

    DriverStatus status = DRIVER_OK;

    switch (HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &id, 1, 100))
    {
    case HAL_TIMEOUT:
        status = DRIVER_COMMUNICATION_ERROR;
        break;
    case HAL_BUSY:
        status = DRIVER_BUSY;
        break;
    case HAL_ERROR:
        status = DRIVER_COMMUNICATION_ERROR;
        break;
    default:
        break;
    }

    if (id == WHO_AM_I_VALUE)
    {
        status = DRIVER_OK;
    }
    else
    {
        status = DRIVER_NOTFOUND;
    }

    LSM6DOS_Reset();

    // Enable INT1 interrupt When GYRO And ACC Data Ready.
    uint8_t int1 = (0x01 | (0x01 << 1));
    HAL_I2C_Mem_Write(&hi2c1, WRITE_ADDRESS, INT1_CTRL, I2C_MEMADD_SIZE_8BIT, &int1, 1, 100);

    return status;
}

void LSM6DOS_Reset()
{
    uint8_t registerStatus;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, CTRL3_C, I2C_MEMADD_SIZE_8BIT, &registerStatus, 1, 100);
    registerStatus |= RESET_MASK;
    HAL_I2C_Mem_Write(&hi2c1, WRITE_ADDRESS, CTRL3_C, I2C_MEMADD_SIZE_8BIT, &registerStatus, 1, 100);
}

LSM6DOS_DEVICE_STATUS LSM6DOS_GetDeviceStatus()
{
    uint8_t statusAcc;
    uint8_t statusGyro;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &statusAcc, 1, 100);
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, CTRL2_G, I2C_MEMADD_SIZE_8BIT, &statusGyro, 1, 100);

    if ((((statusAcc >> 2) & 0x03) == 0x03) || (statusGyro == 0x0B))
    {
        return LSM6DOS_DeviceStatusNotAllowed;
    }
    else if ((statusAcc == POWER_DOWN) && (statusGyro == POWER_DOWN))
    {
        return LSM6DOS_DeviceStatusPowerDown;
    }

    return LSM6DOS_DeviceStatusWork;
}

LSM6DOS_DEVICE_STATUS LSM6DOS_SetWorkStatus(LSM6DOS_WORK_STATUS workStatus)
{
    uint8_t power = 0x02;
    switch (workStatus)
    {
    case LSM6DOS_WorkStatusFullPower:
        power |= WORK_6660HZ_MASK;
        break;
    case LSM6DOS_WorkStatusMiddlePower:
        power |= WORK_208HZ_MASK;
        break;
    case LSM6DOS_WorkStatusLowPower:
        power |= WORK_26HZ_MASK;
        break;
    case LSM6DOS_WorkStatusShutDown:
        power |= POWER_DOWN;
        break;
    default:
        power = POWER_DOWN;
        break;
    }

    // Setting Up Work Mode
    HAL_I2C_Mem_Write(&hi2c1, WRITE_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &power, 1, 100);
    HAL_I2C_Mem_Write(&hi2c1, WRITE_ADDRESS, CTRL2_G, I2C_MEMADD_SIZE_8BIT, &power, 1, 100);

    return LSM6DOS_GetDeviceStatus();
}

uint8_t LSM6DOS_GetDataStatus()
{
    uint8_t regStatus;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, STATUS_REG, I2C_MEMADD_SIZE_8BIT, &regStatus, 1, 100);
    return regStatus;
}

int16_t LSM6DOS_GetTemperature()
{
    int16_t temp;
    uint8_t  tmp;
    tmp = 0;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUT_TEMP_L, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    temp = tmp;
    tmp  = 0;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUT_TEMP_H, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    temp |= (tmp << 8);

    return temp;
}

void LSM6DOS_GetGyro(GYRO_DataStructure *gyro)
{
    uint8_t tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTX_L_G, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    gyro->pitch = tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTX_H_G, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    gyro->pitch |= (tmp << 8);

    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTY_L_G, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    gyro->roll = tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTY_H_G, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    gyro->roll |= (tmp << 8);

    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTZ_L_G, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    gyro->yaw = tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTZ_H_G, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    gyro->yaw |= (tmp << 8);
}

void LSM6DOS_GetAcc(ACC_DataStructure *acc)
{
    uint8_t tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTX_L_A, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    acc->x = tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTX_H_A, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    acc->x |= (tmp << 8);

    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTY_L_A, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    acc->y = tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTY_H_A, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    acc->y |= (tmp << 8);

    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTZ_L_A, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    acc->z = tmp;
    HAL_I2C_Mem_Read(&hi2c1, READ_ADDRESS, OUTZ_H_A, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    acc->z |= (tmp << 8);
}
