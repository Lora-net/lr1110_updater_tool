/*!
 * \file      touch.c
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

#include "touch.h"
#include "system_i2c.h"

#define FT6x06_ADDR 0x70

#define FT6x06_DEV_MODE 0x00
#define FT6x06_GEST_ID 0x01
#define FT6x06_TD_STATUS 0x02
#define FT6x06_TH_GROUP 0x80
#define FT6x06_PERIODACTIVE 0x88
#define FT6x06_CHIPER 0xA3
#define FT6x06_G_MODE 0xA4
#define FT6x06_FIRMID 0xA6
#define FT6x06_FOCALTECH_ID 0xA8
#define FT6x06_RELEASE_CODE_ID 0xAF

void touch_init( void ) { touch_write_register( FT6x06_G_MODE, 0x00 ); }

uint8_t touch_read_register( uint8_t reg )
{
    uint8_t buf[1] = { reg };
    uint8_t data   = 0;

    system_i2c_write( FT6x06_ADDR, buf, 1, true );
    system_i2c_read( FT6x06_ADDR, &data, 1, false );

    return data;
}

void touch_write_register( uint8_t reg, uint8_t val )
{
    uint8_t buf[2] = { reg, val };

    system_i2c_write( FT6x06_ADDR, buf, 2, false );
}

bool touch_is_touched( )
{
    bool    status = false;
    uint8_t val;

    val = touch_read_register( FT6x06_TD_STATUS );
    if( ( val == 1 ) || ( val == 2 ) )
    {
        status = true;
    }

    return status;
}

void readTouchData( uint16_t* posX, uint16_t* posY, bool* touching )
{
    posX[0] = touch_read_register( 0x03 ) & 0x0F;
    posX[0] <<= 8;
    posX[0] |= touch_read_register( 0x04 );
    posY[0] = touch_read_register( 0x05 ) & 0x0F;
    posY[0] <<= 8;
    posY[0] |= touch_read_register( 0x06 );

    posX[0] = map( posX[0], 0, 240, 240, 0 );
    posY[0] = map( posY[0], 0, 320, 320, 0 );

    touching[0] = touch_is_touched( );
}

long map( long x, long in_min, long in_max, long out_min, long out_max )
{
    return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) +
           out_min;
}
