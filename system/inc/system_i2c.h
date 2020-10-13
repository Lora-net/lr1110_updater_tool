/*!
 * \file      system_i2c.h
 *
 * \brief     MCU I2C-related function headers
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

#ifndef __i2c_H
#define __i2c_H

#include <stdbool.h>

#include "stm32l476xx.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

void system_i2c_init( void );
void system_i2c_write( const uint8_t address, const uint8_t* buffer_in,
                       const uint8_t length, const bool repeated );
void system_i2c_read( const uint8_t address, uint8_t* buffer_out,
                      const uint8_t length, const bool repeated );

#ifdef __cplusplus
}
#endif

#endif
