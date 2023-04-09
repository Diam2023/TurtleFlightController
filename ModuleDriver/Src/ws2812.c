/**
 * @file ws2812.c
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 */

#include "ws2812.h"
#include "tim.h"

// WS2812 Numbr
#define PIXEL_NUM 1
#define NUM (24 * PIXEL_NUM + 300)
#define WS1 66
#define WS0 24

/**
 * @brief Callback for PWM DMA Complate
 *
 * @param htim
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1)
    {
        HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    }
}

/**
 * @brief WS2812 Temp
 */
uint16_t buffer[NUM];

/**
 * @brief Flash Data To DMA Data
 *
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-15
 * @copyright Copyright (c) 2023
 */
static void WS2812_Flash()
{
    // Start Transimit
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)buffer, NUM);
}

void WS2812_Initialize()
{
    WS2812_Close();
}

void WS2812_Close()
{
    WS2812_Write(0x00, 0x00, 0x00);
    WS2812_Flash();
}

uint32_t WS2812_Color(uint8_t red, uint8_t green, uint8_t blue)
{
    return green << 16 | red << 8 | blue;
}

void WS2812_SetPixelColor(uint16_t n, uint32_t GRBColor)
{
    uint8_t i;
    if (n < PIXEL_NUM)
    {
        for (i = 0; i < 24; ++i)
        {
            buffer[24 * n + i] = (((GRBColor << i) & 0X800000) ? WS1 : WS0);
        }
    }
}

void WS2812_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t i;

    if (n < PIXEL_NUM)
    {
        for (i = 0; i < 24; ++i)
        {
            buffer[24 * n + i] = (((WS2812_Color(red, green, blue) << i) & 0X800000) ? WS1 : WS0);
        }
    }
}

// void WS2812_Write(uint8_t r, uint8_t g, uint8_t b)
// {
//     WS2812_SetPixelRGB(0, r, g, b);

//     WS2812_Flash();
// }

void WS2812_Write(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t colorData[24];

    for (size_t i = 0; i < 8; i++)
    {
        colorData[i] = ((g & 0x80) ? WS1 : WS0);
        g <<= 1;
    }

    for (size_t i = 0; i < 8; i++)
    {
        colorData[i + 8] = ((r & 0x80) ? WS1 : WS0);
        r <<= 1;
    }

    for (size_t i = 0; i < 8; i++)
    {
        colorData[i + 16] = ((b & 0x80) ? WS1 : WS0);
        b <<= 1;
    }
    //     colorData[i + 8]  = ((r & 0x80) ? WS1 : WS0);
    //     colorData[i + 16] = ((b & 0x80) ? WS1 : WS0);
    //     r <<= 1;
    //     b <<= 1;
    // }

    // for (size_t i = 0; i < 24; i++)
    // {
    //     buffer[24 + i] = colorData[i];
    // }

    for (size_t i = 0; i < PIXEL_NUM; i++)
    {
        for (size_t j = 0; j < 24; j++)
        {
            buffer[i * 24 + j] = colorData[j];
        }
    }

    for (size_t i = (PIXEL_NUM * 24); i < NUM; i++)
    {
        buffer[i] = 0;
    }

    WS2812_Flash();
}

// uint32_t Wheel(uint8_t WheelPos)
// {
// 	WheelPos = 255 - WheelPos;
// 	if (WheelPos < 85)
// 	{
// 		return WS2812_Write(255 - WheelPos * 3, 0, WheelPos * 3);
// 	}
// 	if (WheelPos < 170)
// 	{
// 		WheelPos -= 85;
// 		return WS281x_Color(0, WheelPos * 3, 255 - WheelPos * 3);
// 	}
// 	WheelPos -= 170;
// 	return WS281x_Color(WheelPos * 3, 255 - WheelPos * 3, 0);
// }

// void rainbow(uint8_t wait)
// {
// 	uint32_t timestamp = HAL_GetTick();
// 	uint16_t i;
// 	static uint8_t j;
// 	static uint32_t next_time = 0;

// 	uint32_t flag = 0;
// 	if (next_time < wait)
// 	{
// 		if ((uint64_t)timestamp + wait - next_time > 0)
// 			flag = 1;
// 	}
// 	else if (timestamp > next_time)
// 	{
// 		flag = 1;
// 	}
// 	if (flag) // && (timestamp - next_time < wait*5))
// 	{
// 		j++;
// 		next_time = timestamp + wait;
// 		for (i = 0; i < PIXEL_NUM; i++)
// 		{
// 			WS281x_SetPixelColor(i, Wheel((i + j) & 255));
// 		}
// 	}
// 	WS_Load();
// }

// void rainbowCycle(uint8_t wait)
// {
// 	uint32_t timestamp = HAL_GetTick();
// 	uint16_t i;
// 	static uint8_t j;
// 	static uint32_t next_time = 0;

// 	static uint8_t loop = 0;
// 	if (loop == 0)
// 		next_time = timestamp;
// 	loop = 1; //首次调用初始化

// 	if ((timestamp > next_time)) // && (timestamp - next_time < wait*5))
// 	{
// 		j++;
// 		next_time = timestamp + wait;
// 		for (i = 0; i < PIXEL_NUM; i++)
// 		{
// 			WS281x_SetPixelColor(i, Wheel(((i * 256 / (PIXEL_NUM)) + j) & 255));
// 		}
// 	}
// 	WS_Load();
// }
