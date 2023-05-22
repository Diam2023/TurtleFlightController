/**
 * @file common.c
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "common.h"

unsigned int SizeString(const char *pString)
{
    unsigned int result = 0;
    while (*(pString + result) != '\0')
    {
        result++;
    }

    return result;
}
