/*!
 * @file      lr1110_modem_hal.c
 *
 * @brief     Hardware Abstraction Layer (HAL) implementation for LR1110
 *
 * Revised BSD License
 * Copyright Semtech Corporation 2020. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdlib.h>
#include "lr1110_hal.h"
#include "lr1110_modem_hal.h"
#include "lr1110_modem_system.h"
#include "timer.h"
#include "main.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define LR1110_MODEM_RESET_TIMEOUT 3000
extern SPI_HandleTypeDef hspi2;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief LR1110 modem-e reset timeout flag
 */
static bool lr1110_modem_reset_timeout = false;

/*!
 * @brief Timer to handle the scan timeout
 */
static TimerEvent_t lr1110_modem_reset_timeout_timer;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Function to wait that the lr1110 transceiver busy line raise to high
 *
 * @param [in] context Chip implementation context
 * @param [in] timeout_ms timeout in millisec before leave the function
 *
 * @returns lr1110_hal_status_t
 */
static lr1110_hal_status_t lr1110_hal_wait_on_busy( const void* context, uint32_t timeout_ms );

/*!
 * @brief Function to wait that the lr1110 modem-e busy line fall to low
 *
 * @param [in] context Chip implementation context
 * @param [in] timeout_ms timeout in millisec before leave the function
 *
 * @returns lr1110_hal_status_t
 */
static lr1110_modem_hal_status_t lr1110_modem_hal_wait_on_busy( const void* context, uint32_t timeout_ms );

/*!
 * @brief Function to wait the that lr1110 modem-e busy line raise to high
 *
 * @param [in] context Chip implementation context
 * @param [in] timeout_ms timeout in millisec before leave the function
 *
 * @returns lr1110_hal_status_t
 */
static lr1110_modem_hal_status_t lr1110_modem_hal_wait_on_unbusy( const void* context, uint32_t timeout_ms );

/*!
 * @brief Function executed on lr1110 modem-e reset timeout event
 */
static void on_lr1110_modem_reset_timeout_event( void* context );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/*!
 * @brief lr1110_modem_hal.h API implementation
 */

lr1110_modem_hal_status_t lr1110_modem_hal_write( const void* context, const uint8_t* command,
                                                  const uint16_t command_length, const uint8_t* data,
                                                  const uint16_t data_length )
{

    uint8_t                   crc          = 0;
    lr1110_modem_hal_status_t status;
    uint8_t rbuffer[500];

    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET )//Busy pin = PB1
    {
      	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);//NSS pin = PB12
        HAL_SPI_TransmitReceive(&hspi2, 0, rbuffer, 1,3000);
      	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    }

    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);

    crc = lr1110_modem_compute_crc( 0xFF, command, command_length );
    crc = lr1110_modem_compute_crc( crc, data, data_length );

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, command, rbuffer, command_length,3000);
    HAL_SPI_TransmitReceive(&hspi2, data, rbuffer, data_length,3000);
    HAL_SPI_TransmitReceive(&hspi2, &crc, rbuffer, 1,3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, 0, &status, 1,3000);
    HAL_SPI_TransmitReceive(&hspi2, 0, &crc, 1,3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    return LR1110_MODEM_HAL_STATUS_OK;
}

lr1110_modem_hal_status_t lr1110_modem_hal_read( const void* context, const uint8_t* command,
                                                 const uint16_t command_length, uint8_t* data,
                                                 const uint16_t data_length )
{
        uint8_t                   crc          = 0;
        lr1110_modem_hal_status_t status;
        uint8_t rbuf[500];
        uint8_t* rbuffer;
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET )//Busy pin = PB1
        {
          	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);//NSS pin = PB12
            HAL_SPI_TransmitReceive(&hspi2, &crc, rbuffer, 1,10000);
          	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        }
        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);

        crc = lr1110_modem_compute_crc( 0xFF, command, command_length );

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&hspi2, command, rbuf, command_length,3000);
        HAL_SPI_TransmitReceive(&hspi2, crc, rbuffer, 1,3000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET);
        crc=0;

       /* NSS low */
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
       HAL_SPI_TransmitReceive(&hspi2, 0, &status, 1,3000);
       HAL_SPI_TransmitReceive(&hspi2, 0, data, data_length,3000);
       HAL_SPI_TransmitReceive(&hspi2, 0, ( uint8_t* ) &crc, 1 ,3000);
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
       return LR1110_MODEM_HAL_STATUS_OK;
}

lr1110_modem_hal_status_t lr1110_modem_hal_reset( const void* context )
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    return LR1110_MODEM_HAL_STATUS_OK;
}

void lr1110_modem_hal_enter_dfu( const void* context ) {}
lr1110_modem_hal_status_t lr1110_modem_hal_wakeup( const void* context ) { return LR1110_MODEM_HAL_STATUS_BAD_FRAME; }

lr1110_modem_hal_status_t lr1110_bootloader_hal_wakeup( const void* context )
{
    return LR1110_MODEM_HAL_STATUS_BAD_FRAME;
}
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
