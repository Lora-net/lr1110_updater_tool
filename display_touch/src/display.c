/*!
 * \file      display.c
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

#include "configuration.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_utils.h"
#include "system.h"
#include "display.h"

void display_send_command( const uint8_t command )
{
    LL_GPIO_ResetOutputPin( DISPLAY_DC_PORT, DISPLAY_DC_PIN );
    system_spi_write( SPI1, &command, 1 );
    LL_GPIO_SetOutputPin( DISPLAY_DC_PORT, DISPLAY_DC_PIN );
}

void display_send_data_8bit( const uint8_t data )
{
    system_spi_write( SPI1, &data, 1 );
}

void display_send_data( const uint16_t data )
{
    uint8_t dh = data >> 8;
    uint8_t dl = data & 0xff;

    system_spi_write( SPI1, &dh, 1 );
    system_spi_write( SPI1, &dl, 1 );
}

void display_init( void )
{
    LL_GPIO_ResetOutputPin( DISPLAY_NSS_PORT, DISPLAY_NSS_PIN );

    // ILI9341 init
    display_send_command( 0x11 );

    display_send_command( 0xCF );
    display_send_data_8bit( 0x00 );
    display_send_data_8bit( 0xc3 );
    display_send_data_8bit( 0X30 );

    display_send_command( 0xED );
    display_send_data_8bit( 0x64 );
    display_send_data_8bit( 0x03 );
    display_send_data_8bit( 0X12 );
    display_send_data_8bit( 0X81 );

    display_send_command( 0xE8 );
    display_send_data_8bit( 0x85 );
    display_send_data_8bit( 0x10 );
    display_send_data_8bit( 0x79 );

    display_send_command( 0xCB );
    display_send_data_8bit( 0x39 );
    display_send_data_8bit( 0x2C );
    display_send_data_8bit( 0x00 );
    display_send_data_8bit( 0x34 );
    display_send_data_8bit( 0x02 );

    display_send_command( 0xF7 );
    display_send_data_8bit( 0x20 );

    display_send_command( 0xEA );
    display_send_data_8bit( 0x00 );
    display_send_data_8bit( 0x00 );

    display_send_command( 0xC0 );    // Power control
    display_send_data_8bit( 0x22 );  // VRH[5:0]

    display_send_command( 0xC1 );    // Power control
    display_send_data_8bit( 0x11 );  // SAP[2:0];BT[3:0]

    display_send_command( 0xC5 );  // VCM control
    display_send_data_8bit( 0x3d );
    display_send_data_8bit( 0x20 );

    display_send_command( 0xC7 );    // VCM control2
    display_send_data_8bit( 0xAA );  // 0xB0

    display_send_command( 0x36 );  // Memory Access Control
    display_send_data_8bit(
        0xC8 );  // display_send_data_8bit(0x08); if not inverted

    display_send_command( 0x3A );
    display_send_data_8bit( 0x55 );

    display_send_command( 0xB1 );
    display_send_data_8bit( 0x00 );
    display_send_data_8bit( 0x13 );

    display_send_command( 0xB6 );  // Display Function Control
    display_send_data_8bit( 0x0A );
    display_send_data_8bit( 0xA2 );

    display_send_command( 0xF6 );
    display_send_data_8bit( 0x01 );
    display_send_data_8bit( 0x30 );

    display_send_command( 0xF2 );  // 3Gamma Function Disable
    display_send_data_8bit( 0x00 );

    display_send_command( 0x26 );  // Gamma curve selected
    display_send_data_8bit( 0x01 );

    display_send_command( 0xE0 );  // Set Gamma
    display_send_data_8bit( 0x0F );
    display_send_data_8bit( 0x3F );
    display_send_data_8bit( 0x2F );
    display_send_data_8bit( 0x0C );
    display_send_data_8bit( 0x10 );
    display_send_data_8bit( 0x0A );
    display_send_data_8bit( 0x53 );
    display_send_data_8bit( 0XD5 );
    display_send_data_8bit( 0x40 );
    display_send_data_8bit( 0x0A );
    display_send_data_8bit( 0x13 );
    display_send_data_8bit( 0x03 );
    display_send_data_8bit( 0x08 );
    display_send_data_8bit( 0x03 );
    display_send_data_8bit( 0x00 );

    display_send_command( 0XE1 );  // Set Gamma
    display_send_data_8bit( 0x00 );
    display_send_data_8bit( 0x00 );

    display_send_data_8bit( 0x10 );
    display_send_data_8bit( 0x03 );
    display_send_data_8bit( 0x0F );
    display_send_data_8bit( 0x05 );
    display_send_data_8bit( 0x2C );
    display_send_data_8bit( 0xA2 );
    display_send_data_8bit( 0x3F );
    display_send_data_8bit( 0x05 );
    display_send_data_8bit( 0x0E );
    display_send_data_8bit( 0x0C );
    display_send_data_8bit( 0x37 );
    display_send_data_8bit( 0x3C );
    display_send_data_8bit( 0x0F );

    display_send_command( 0x11 );  // Exit Sleep

    display_send_command( 0x29 );  // Display on

    LL_GPIO_SetOutputPin( DISPLAY_NSS_PORT, DISPLAY_NSS_PIN );

    LL_mDelay( 5 );
}
