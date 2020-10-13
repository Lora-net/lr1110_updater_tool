/*!
 * \file      lr1110_hal.c
 *
 * \brief     HAL implementation for LR1110 radio chip
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

#include "lr1110_modem_hal.h"
#include "configuration.h"
#include "system.h"

lr1110_modem_hal_status_t lr1110_modem_hal_write( const void* context, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length )
{
    radio_t*                  radio_local = ( radio_t* ) context;
    uint8_t                   crc         = 0;
    lr1110_modem_hal_status_t rc;

    if( system_gpio_get_pin_state( radio_local->busy ) == SYSTEM_GPIO_PIN_STATE_HIGH )
    {
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
        system_spi_write( radio_local->spi, &crc, 1 );
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );
    }

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    crc = lr1110_modem_compute_crc( 0xFF, command, command_length );
    crc = lr1110_modem_compute_crc( crc, data, data_length );

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_write( radio_local->spi, command, command_length );
    system_spi_write( radio_local->spi, data, data_length );
    system_spi_write( radio_local->spi, &crc, 1 );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_HIGH );

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_read( radio_local->spi, ( uint8_t* ) &rc, 1 );
    system_spi_read( radio_local->spi, ( uint8_t* ) &crc, 1 );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_MODEM_HAL_STATUS_OK;
}

lr1110_modem_hal_status_t lr1110_bootloader_hal_write( const void* context, const uint8_t* command,
                                                       const uint16_t command_length, const uint8_t* data,
                                                       const uint16_t data_length )
{
    return LR1110_MODEM_HAL_STATUS_BAD_FRAME;
}

lr1110_modem_hal_status_t lr1110_modem_hal_read( const void* context, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length )
{
    radio_t*                  radio_local = ( radio_t* ) context;
    uint8_t                   crc         = 0;
    lr1110_modem_hal_status_t rc          = 0;
    ;

    if( system_gpio_get_pin_state( radio_local->busy ) == SYSTEM_GPIO_PIN_STATE_HIGH )
    {
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
        system_spi_write( radio_local->spi, &crc, 1 );
        system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );
    }

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    crc = lr1110_modem_compute_crc( 0xFF, command, command_length );

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_write( radio_local->spi, command, command_length );
    system_spi_write( radio_local->spi, &crc, 1 );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_HIGH );

    crc = 0;

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_read( radio_local->spi, ( uint8_t* ) &rc, 1 );
    system_spi_read( radio_local->spi, data, data_length );
    system_spi_read( radio_local->spi, ( uint8_t* ) &crc, 1 );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_MODEM_HAL_STATUS_OK;
}

lr1110_modem_hal_status_t lr1110_bootloader_hal_read( const void* context, const uint8_t* command,
                                                      const uint16_t command_length, uint8_t* data,
                                                      const uint16_t data_length )
{
    return LR1110_MODEM_HAL_STATUS_BAD_FRAME;
}

lr1110_modem_hal_status_t lr1110_modem_hal_write_read( const void* context, const uint8_t* command, uint8_t* data,
                                                       const uint16_t data_length )
{
    return LR1110_MODEM_HAL_STATUS_BAD_FRAME;
}

lr1110_modem_hal_status_t lr1110_modem_hal_reset( const void* context )
{
    radio_t* radio_local = ( radio_t* ) context;

    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 1 );
    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_MODEM_HAL_STATUS_OK;
}

void lr1110_modem_hal_enter_dfu( const void* context ) {}

lr1110_modem_hal_status_t lr1110_modem_hal_wakeup( const void* context ) { return LR1110_MODEM_HAL_STATUS_BAD_FRAME; }

lr1110_modem_hal_status_t lr1110_bootloader_hal_wakeup( const void* context )
{
    return LR1110_MODEM_HAL_STATUS_BAD_FRAME;
}