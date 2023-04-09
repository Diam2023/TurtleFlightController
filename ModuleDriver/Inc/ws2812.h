/**
 * @file ws2812.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TURTLE_FLIGHT_WS2812_H
#define TURTLE_FLIGHT_WS2812_H

#include <stdint.h>

/**
 * @brief Initialize
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-14
 * @copyright Copyright (c) 2023
 */
void WS2812_Initialize();

/**
 * @brief
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-14
 * @copyright Copyright (c) 2023
 */
void WS2812_Close();

/**
 * @brief Write RGB Color For WS2812
 *
 * @param r red
 * @param g green
 * @param b blue
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-14
 * @copyright Copyright (c) 2023
 */
void WS2812_Write(uint8_t r, uint8_t g, uint8_t b);

#endif // TURTLE_FLIGHT_WS2812_H
