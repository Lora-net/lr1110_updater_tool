/*!
 * \file      system.c
 *
 * \brief     MCU system-related functions
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

#include "system.h"

void system_init( void )
{
    system_clock_init( );
    system_gpio_init( );
    system_spi_init( );
    // system_i2c_init( );
    system_time_init( );
    system_uart_init( );
    // system_lptim_init( );
}
