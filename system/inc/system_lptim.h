/*!
 * \file      system_lptim.h
 *
 * \brief     MCU low-power timer related functions header
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

#ifndef __SYSTEM_LPTIM_H__
#define __SYSTEM_LPTIM_H__

#include "stdint.h"
#include "stm32l4xx_ll_lptim.h"

#ifdef __cplusplus
extern "C" {
#endif

void system_lptim_init( );
void system_lptim_set_and_run( uint32_t ticks );

#ifdef __cplusplus
}
#endif

#endif