/*!
 * \file      system_clock.c
 *
 * \brief     MCU clock-related functions
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

#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_bus.h"

void system_clock_init( void )
{
    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SYSCFG );
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );

    LL_FLASH_SetLatency( LL_FLASH_LATENCY_4 );

    if( LL_FLASH_GetLatency( ) != LL_FLASH_LATENCY_4 )
    {
        // Error_Handler();
    }

    LL_PWR_SetRegulVoltageScaling( LL_PWR_REGU_VOLTAGE_SCALE1 );

    LL_RCC_HSI_Enable( );

    while( LL_RCC_HSI_IsReady( ) != 1 )
    {
    }

    LL_RCC_HSI_SetCalibTrimming( 16 );

    LL_PWR_EnableBkUpAccess( );
    LL_RCC_LSE_SetDriveCapability( LL_RCC_LSEDRIVE_LOW );
    LL_RCC_LSE_Enable( );

    while( LL_RCC_LSE_IsReady( ) != 1 )
    {
    }

    LL_RCC_PLL_ConfigDomain_SYS( LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10,
                                 LL_RCC_PLLR_DIV_2 );
    LL_RCC_PLL_ConfigDomain_48M( LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10,
                                 LL_RCC_PLLQ_DIV_4 );
    LL_RCC_PLL_EnableDomain_48M( );
    LL_RCC_PLL_EnableDomain_SYS( );
    LL_RCC_PLL_Enable( );
    while( LL_RCC_PLL_IsReady( ) != 1 )
    {
    }

    LL_RCC_SetSysClkSource( LL_RCC_SYS_CLKSOURCE_PLL );
    while( LL_RCC_GetSysClkSource( ) != LL_RCC_SYS_CLKSOURCE_STATUS_PLL )
    {
    }

    LL_RCC_SetAHBPrescaler( LL_RCC_SYSCLK_DIV_1 );
    LL_RCC_SetAPB1Prescaler( LL_RCC_APB1_DIV_1 );
    LL_RCC_SetAPB2Prescaler( LL_RCC_APB2_DIV_1 );

    LL_Init1msTick( 80000000 );
    LL_SYSTICK_SetClkSource( LL_SYSTICK_CLKSOURCE_HCLK );
    LL_SetSystemCoreClock( 80000000 );

    LL_RCC_SetUSARTClockSource( LL_RCC_USART2_CLKSOURCE_PCLK1 );
    LL_RCC_SetRNGClockSource( LL_RCC_RNG_CLKSOURCE_PLL );
    LL_RCC_SetLPTIMClockSource( LL_RCC_LPTIM1_CLKSOURCE_LSE );
}
