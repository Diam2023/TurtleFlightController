/**
 * @file status_light.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TURTLE_FLIGHT_STATUS_LIGHT_H
#define TURTLE_FLIGHT_STATUS_LIGHT_H

#include "common.h"

// Status Of Turtle
typedef enum
{
    STATUS_ERROR  = 0, 
    STATUS_NORMAL = 1,
    STATUS_DEBUG  = 2,
    STATUS_VIEWE  = 3
} TurtleStatus;

typedef enum
{
    GROY_ERROR    = 0,
    RECIVER_ERROR = 1,
    UNKNOW_ERROR,

} TurtleErrorType;

/**
 * @brief Status Light For Turtle Flight Controller
 *
 * @return true Successful Initialize Status Light
 * @return false Error Initialize Status Light
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-14
 * @copyright Copyright (c) 2023
 */
bool InitializeStatusLight();

#endif // TURTLE_FLIGHT_STATUS_LIGHT_H