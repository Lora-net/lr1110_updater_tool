/*!
 * \file      system_uart.c
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

#include "system_uart.h"
#include "stm32l4xx_ll_dma.h"

#ifndef NULL
#define NULL ( 0 )
#endif

static void system_uart_dma_configure_tx( uint8_t* buffer,
                                          uint32_t buffer_size );
static void system_uart_dma_configure_rx( uint8_t* buffer,
                                          uint32_t buffer_size );

void system_uart_init( )
{
    LL_USART_InitTypeDef USART_InitStruct = { 0 };

    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_USART2 );

    LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOA );
    /**USART2 GPIO Configuration
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX
    */
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate  = LL_GPIO_AF_7;
    LL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    USART_InitStruct.BaudRate            = 921600;
    USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
    LL_USART_Init( USART2, &USART_InitStruct );
    LL_USART_ConfigAsyncMode( USART2 );
    LL_USART_Enable( USART2 );

    while( LL_USART_IsEnabled( USART2 ) == 0 )
        ;
}

int32_t system_uart_send_char( int32_t ch )
{
    while( !LL_USART_IsActiveFlag_TXE( USART2 ) )
        ;
    LL_USART_TransmitData8( USART2, ch & 0xFF );

    return ch;
}

int32_t system_uart_receive_char( void )
{
    int32_t ret = -1;

    while( !LL_USART_IsActiveFlag_RXNE( USART2 ) )
        ;
    ret = LL_USART_ReceiveData8( USART2 );

    return ret;
}

void system_uart_flush( void ) { LL_USART_RequestRxDataFlush( USART2 ); }
