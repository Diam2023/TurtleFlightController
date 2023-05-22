/**
 * @file bmp280.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief BMP280 Low Level Driver
 * @version 0.1
 * @date 2023-03-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef TURTLE_FLIGHT_BMP280_H
#define TURTLE_FLIGHT_BMP280_H

#include <stdint.h>
#include "common.h"

bool BMP280_Check();
uint8_t BMP280_GetId();


void BMP280_Reset();

DriverStatus BMP280_Initialize();



#endif // TURTLE_FLIGHT_BMP280_H
