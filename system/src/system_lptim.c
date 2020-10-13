/*!
 * \file      system_lptim.c
 *
 * \brief     MCU low-power timer related functions
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

#include "system_lptim.h"
#include "stm32l4xx_ll_bus.h"

void system_lptim_init( )
{
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_LPTIM1 );

    NVIC_SetPriority( LPTIM1_IRQn, 0 );
    NVIC_EnableIRQ( LPTIM1_IRQn );

    LL_LPTIM_SetClockSource( LPTIM1, LL_LPTIM_CLK_SOURCE_INTERNAL );
    LL_LPTIM_SetPrescaler( LPTIM1, LL_LPTIM_PRESCALER_DIV16 );
    LL_LPTIM_SetPolarity( LPTIM1, LL_LPTIM_OUTPUT_POLARITY_REGULAR );
    LL_LPTIM_SetUpdateMode( LPTIM1, LL_LPTIM_UPDATE_MODE_IMMEDIATE );
    LL_LPTIM_SetCounterMode( LPTIM1, LL_LPTIM_COUNTER_MODE_INTERNAL );
    LL_LPTIM_TrigSw( LPTIM1 );
    LL_LPTIM_SetInput1Src( LPTIM1, LL_LPTIM_INPUT1_SRC_GPIO );
    LL_LPTIM_SetInput2Src( LPTIM1, LL_LPTIM_INPUT2_SRC_GPIO );

    LL_LPTIM_Enable( LPTIM1 );

    while( LL_LPTIM_IsEnabled( LPTIM1 ) != 1 )
    {
    }

    LL_LPTIM_ClearFLAG_ARRM( LPTIM1 );
    LL_LPTIM_EnableIT_ARRM( LPTIM1 );
}

void system_lptim_set_and_run( uint32_t ticks )
{
    LL_LPTIM_SetAutoReload( LPTIM1, ticks );
    LL_LPTIM_StartCounter( LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT );
}