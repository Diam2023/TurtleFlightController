/**
 * @file w25q16.h
 * @author Monoliths (monoliths-uni@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-03-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TURTLE_FLIGHT_W25Q16_H
#define TURTLE_FLIGHT_W25Q16_H

#include "common.h"

/**
 * @brief  W25Q128FV Configuration
 */
#define W25Q128FV_FLASH_SIZE 0x1000000  /* 128 MBits => 16MBytes */
#define W25Q128FV_SECTOR_SIZE 0x10000   /* 256 sectors of 64KBytes */
#define W25Q128FV_SUBSECTOR_SIZE 0x1000 /* 4096 subsectors of 4kBytes */
#define W25Q128FV_PAGE_SIZE 0x100       /* 65536 pages of 256 bytes */

#define W25Q128FV_DUMMY_CYCLES_READ 4
#define W25Q128FV_DUMMY_CYCLES_READ_QUAD 10

#define W25Q128FV_BULK_ERASE_MAX_TIME 250000
#define W25Q128FV_SECTOR_ERASE_MAX_TIME 3000
#define W25Q128FV_SUBSECTOR_ERASE_MAX_TIME 800
#define W25Qx_TIMEOUT_VALUE 1000

/**
 * @brief  W25Q128FV Commands
 */
/* Reset Operations */
#define RESET_ENABLE_CMD 0x66
#define RESET_MEMORY_CMD 0x99

#define ENTER_QPI_MODE_CMD 0x38
#define EXIT_QPI_MODE_CMD 0xFF

/* Identification Operations */
#define READ_ID_CMD 0x90
#define DUAL_READ_ID_CMD 0x92
#define QUAD_READ_ID_CMD 0x94
#define READ_JEDEC_ID_CMD 0x9F

/* Read Operations */
#define READ_CMD 0x03
#define FAST_READ_CMD 0x0B
#define DUAL_OUT_FAST_READ_CMD 0x3B
#define DUAL_INOUT_FAST_READ_CMD 0xBB
#define QUAD_OUT_FAST_READ_CMD 0x6B
#define QUAD_INOUT_FAST_READ_CMD 0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD 0x06
#define WRITE_DISABLE_CMD 0x04

/* Register Operations */
#define READ_STATUS_REG1_CMD 0x05
#define READ_STATUS_REG2_CMD 0x35
#define READ_STATUS_REG3_CMD 0x15

#define WRITE_STATUS_REG1_CMD 0x01
#define WRITE_STATUS_REG2_CMD 0x31
#define WRITE_STATUS_REG3_CMD 0x11

/* Program Operations */
#define PAGE_PROG_CMD 0x02
#define QUAD_INPUT_PAGE_PROG_CMD 0x32

/* Erase Operations */
#define SECTOR_ERASE_CMD 0x20
#define CHIP_ERASE_CMD 0xC7

#define PROG_ERASE_RESUME_CMD 0x7A
#define PROG_ERASE_SUSPEND_CMD 0x75

/* Flag Status Register */
#define W25Q128FV_FSR_BUSY ((uint8_t)0x01) /*!< busy */
#define W25Q128FV_FSR_WREN ((uint8_t)0x02) /*!< write enable */
#define W25Q128FV_FSR_QE ((uint8_t)0x02)   /*!< quad enable */

/**
 * @brief Initialzie W25Q16
 *
 * @return DriverStatus
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-16
 * @copyright Copyright (c) 2023
 */
DriverStatus W25Q16_Initialize();

/**
 * @brief Get Device Id For W25Q16
 *
 * @return unsigned short
 * @version 0.1
 * @author Monoliths (monoliths-uni@outlook.com)
 * @date 2023-03-16
 * @copyright Copyright (c) 2023
 */
unsigned short W25Q16_GetId();

#endif // TURTLE_FLIGHT_W25Q16_H
