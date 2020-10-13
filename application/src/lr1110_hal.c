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

#include "lr1110_hal.h"
#include "configuration.h"
#include "system.h"

lr1110_hal_status_t lr1110_hal_reset( const void* radio )
{
    radio_t* radio_local = ( radio_t* ) radio;

    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 1 );
    system_gpio_set_pin_state( radio_local->reset, SYSTEM_GPIO_PIN_STATE_HIGH );
  
    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_wakeup( const void* radio )
{
    radio_t* radio_local = ( radio_t* ) radio;

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 1 );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_read( const void* radio, const uint8_t* cbuffer,
                                     const uint16_t cbuffer_length,
                                     uint8_t*       rbuffer,
                                     const uint16_t rbuffer_length )
{
    radio_t* radio_local = ( radio_t* ) radio;
    uint8_t  dummy_byte  = 0x00;

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    /* 1st SPI transaction */
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_write( radio_local->spi, cbuffer, cbuffer_length );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    /* 2nd SPI transaction */
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_write( radio_local->spi, &dummy_byte, 1 );
    system_spi_read( radio_local->spi, rbuffer, rbuffer_length );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_write( const void* radio, const uint8_t* cbuffer,
                                      const uint16_t cbuffer_length,
                                      const uint8_t* cdata,
                                      const uint16_t cdata_length )
{
    radio_t* radio_local = ( radio_t* ) radio;

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_write( radio_local->spi, cbuffer, cbuffer_length );
    system_spi_write( radio_local->spi, cdata, cdata_length );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}

lr1110_hal_status_t lr1110_hal_write_read( const void*    radio,
                                           const uint8_t* cbuffer,
                                           uint8_t*       rbuffer,
                                           const uint16_t length )
{
    radio_t* radio_local = ( radio_t* ) radio;

    system_gpio_wait_for_state( radio_local->busy, SYSTEM_GPIO_PIN_STATE_LOW );

    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_LOW );
    system_spi_write_read( radio_local->spi, cbuffer, rbuffer, length );
    system_gpio_set_pin_state( radio_local->nss, SYSTEM_GPIO_PIN_STATE_HIGH );

    return LR1110_HAL_STATUS_OK;
}
