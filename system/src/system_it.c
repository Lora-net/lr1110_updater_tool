/*!
 * \file      system_it.c
 *
 * \brief     MCU interrupt-related functions
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

/* Includes ------------------------------------------------------------------*/
#include "system_it.h"
#include "stm32l4xx_ll_dma.h"
#include "configuration.h"
#include <stdbool.h>
#include "system_time.h"
#include "system_lptim.h"
#include "system_uart.h"

extern void lv_tick_inc( uint32_t );

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler( void ) {}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler( void )
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while( 1 )
    {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler( void )
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while( 1 )
    {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler( void )
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while( 1 )
    {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler( void )
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while( 1 )
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler( void ) {}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler( void ) {}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler( void ) {}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler( void )
{
    system_time_IncreaseTicker( );
    lv_tick_inc( 1 );
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (EXTI), for the */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/

// /**
//  * @brief  This function handles external lines 10 to 15 interrupt request.
//  * @param  None
//  * @retval None
//  */

// void EXTI4_IRQHandler( void ) { LL_EXTI_ClearFlag_0_31( LL_EXTI_LINE_4 ); }

// /**
//  * @brief  This function handles external lines 10 to 15 interrupt request.
//  * @param  None
//  * @retval None
//  */
// void EXTI15_10_IRQHandler( void )
// {
//     if( LL_EXTI_IsActiveFlag_0_31( LL_EXTI_LINE_10 ) )
//     {
//         LL_EXTI_ClearFlag_0_31( LL_EXTI_LINE_10 );
//     }
// }

// /**
//  * @brief  This function handles DMA1 interrupt request.
//  * @param  None
//  * @retval None
//  */
// void DMA1_Channel7_IRQHandler( void )
// {
//     if( LL_DMA_IsActiveFlag_TC7( DMA1 ) )
//     {
//         LL_DMA_ClearFlag_GI7( DMA1 );
//     }
//     else if( LL_DMA_IsActiveFlag_TE7( DMA1 ) )
//     {
//     }
// }

// /**
//  * @brief  This function handles DMA1 interrupt request.
//  * @param  None
//  * @retval None
//  */
// void DMA1_Channel6_IRQHandler( void )
// {
//     if( LL_DMA_IsActiveFlag_TC6( DMA1 ) )
//     {
//         LL_DMA_ClearFlag_GI6( DMA1 );
//     }
//     else if( LL_DMA_IsActiveFlag_TE6( DMA1 ) )
//     {
//     }
// }

// /**
//  * @brief This function handles LPTIM1 global interrupt.
//  */
// void LPTIM1_IRQHandler( void )
// {
//     /* Check whether Autoreload match interrupt is pending */
//     if( LL_LPTIM_IsActiveFlag_ARRM( LPTIM1 ) == 1 )
//     {
//         /* Clear the Autoreload match interrupt flag */
//         LL_LPTIM_ClearFLAG_ARRM( LPTIM1 );
//     }
// }
