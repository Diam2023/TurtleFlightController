/**
 * @file common.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef TURTLE_FLIGHT_COMMON_H
#define TURTLE_FLIGHT_COMMON_H

#ifndef __cplusplus


// Base Type
typedef enum {
    false = 0,
    true = 1
} bool;

#endif

// for class
#define public
#define private
#define protected


typedef enum
{
    DRIVER_NOTFOUND = 0,
    DRIVER_BUSY = 1,
    DRIVER_OK = 2,
    DRIVER_COMMUNICATION_ERROR = 3
} DriverStatus;


/**
 * @brief Calculat String Length
 * 
 * @param pString string
 * @return size_t length
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
unsigned int SizeString(const char* pString);

#endif // TURTLE_FLIGHT_COMMON_H

