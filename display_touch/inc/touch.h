/*!
 * \file      touch.h
 *
 * \brief
 *
 * \copyright Copyright Semtech Corporation 2019. All rights reserved.
 *
 * Disclaimer: “The Semtech software and the documentation are provided "as is"
 * without any express or implied warranty of any kind from Semtech or from any
 * other person or entity, including warranties of merchantability,
 * noninfringement, or fitness for a particular purpose.”
 *
 * \author    Semtech WSP Applications Team
 */

#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <stdint.h>
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

void touch_init( void );

uint8_t touch_read_register( uint8_t reg );

void touch_write_register( uint8_t reg, uint8_t val );

bool touch_is_touched( void );

void readTouchData( uint16_t* posX, uint16_t* posY, bool* touching );

long map( long x, long in_min, long in_max, long out_min, long out_max );

#ifdef __cplusplus
}
#endif

#endif
