/*!
 * \file      system_uart.h
 *
 * \brief     MCU UART-related functions header file
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

#ifndef _SYSTEM_UART_H
#define _SYSTEM_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "stm32l476xx.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"

void    system_uart_init( void );
int32_t system_uart_send_char( int32_t ch );
void    system_uart_start_receiving( void );
void    system_uart_stop_receiving( void );
int32_t system_uart_receive_char( void );
uint8_t system_uart_is_readable( void );
void    system_uart_flush( void );

void system_uart_dma_init( void );
void system_uart_dma_deinit( void );
bool system_uart_send_buffer( uint8_t* data, uint16_t size );
bool system_uart_is_tx_terminated( void );
void system_uart_start_buffer_reception( const uint16_t size,
                                         uint8_t*       rx_buffer );
void system_uart_register_rx_done_callback( void* object,
                                            void ( *callback )( void* ) );
void system_uart_register_tx_done_callback( void* object,
                                            void ( *callback )( void* ) );
void system_uart_unregister_rx_done_callback( void );
void system_uart_unregister_tx_done_callback( void );
void system_uart_reset( void );
void system_uart_dma_tx_complete_callback( void );
void system_uart_dma_rx_complete_callback( void );
void system_uart_dma_txrx_error( void );

#ifdef __cplusplus
}
#endif

#endif
