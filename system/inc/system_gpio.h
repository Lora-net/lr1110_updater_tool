/*!
 * \file      system_gpio.h
 *
 * \brief     MCU GPIO-related functions header file
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

#ifndef _SYSTEM_GPIO_H
#define _SYSTEM_GPIO_H

#include "configuration.h"

typedef enum
{
    SYSTEM_GPIO_NO_INTERRUPT = 0,
    SYSTEM_GPIO_RISING,
    SYSTEM_GPIO_FALLING,
    SYSTEM_GPIO_BOTH,
} system_gpio_interrupt_t;

typedef enum
{
    SYSTEM_GPIO_PIN_ANTENNA_SELECTOR_CTRL,
    SYSTEM_GPIO_PIN_ANTENNA_SELECTOR_N_CTRL,
    SYSTEM_GPIO_PIN_LED_SCAN,
} system_gpio_pin_t;

typedef enum
{
    SYSTEM_GPIO_PIN_STATE_LOW,
    SYSTEM_GPIO_PIN_STATE_HIGH,
} system_gpio_pin_state_t;

typedef enum
{
    SYSTEM_GPIO_PIN_DIRECTION_INPUT,
    SYSTEM_GPIO_PIN_DIRECTION_OUTPUT,
} system_gpio_pin_direction_t;

#ifdef __cplusplus
extern "C" {
#endif

void                    system_gpio_init( void );
void                    system_gpio_set_pin_state( gpio_t                        gpio,
                                                   const system_gpio_pin_state_t state );
system_gpio_pin_state_t system_gpio_get_pin_state( gpio_t gpio );
void                    system_gpio_init_direction_state(
                       const gpio_t gpio, const system_gpio_pin_direction_t direction,
                       const system_gpio_pin_state_t state );
void system_gpio_wait_for_state( gpio_t io, uint8_t state );

#ifdef __cplusplus
}
#endif

#endif
