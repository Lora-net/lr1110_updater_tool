/*!
 * \file      system_spi.c
 *
 * \brief     MCU SPI-related functions header file
 *
 * \copyright Revised BSD License, see file LICENSE.txt
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2019 Semtech
 *
 * \endcode
 *
 * \authors   Semtech WSP Applications Team
 */

#ifndef _SYSTEM_SPI_H
#define _SYSTEM_SPI_H

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_spi.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void system_spi_init( void );

void system_spi_write( SPI_TypeDef* spi, const uint8_t* buffer,
                       uint16_t length );
void system_spi_read( SPI_TypeDef* spi, uint8_t* buffer, uint16_t length );
void system_spi_write_read( SPI_TypeDef* spi, const uint8_t* cbuffer,
                            uint8_t* rbuffer, uint16_t length );

#ifdef __cplusplus
}
#endif

#endif
