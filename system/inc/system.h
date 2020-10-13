/*!
 * \file      system.h
 *
 * \brief     MCU system-related functions header
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

#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system_clock.h"
#include "system_gpio.h"
#include "system_spi.h"
#include "system_uart.h"
#include "system_i2c.h"
#include "system_time.h"
#include "system_lptim.h"

void system_init( void );

#endif

#ifdef __cplusplus
}
#endif
