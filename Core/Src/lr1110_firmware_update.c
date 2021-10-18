/*!
 * \file      lr1110_firmware_update.c
 *
 * \brief     LR1110 firmware update implementation
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

#include <stdio.h>

#include "lr1110_bootloader.h"
#include "lr1110_system.h"
#include "lr1110_firmware_update.h"
#include "lr1110_modem_lorawan.h"
//#include "system.h"
#include "main.h"
#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

lr1110_fw_update_status_t lr1110_update_firmware( void* radio, lr1110_fw_update_t fw_update_direction,
                                                  uint32_t fw_expected, const uint32_t* buffer, uint32_t length )
{
    lr1110_fw_update_status_t   status             = LR1110_FW_UPDATE_ERROR;
    lr1110_bootloader_version_t version_bootloader = { 0 };
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    char data[100];


    lr1110_system_stat1_t stat1;
    lr1110_system_stat2_t stat2;
    uint32_t              irq;

    printf( "Reset the chip...\n" );

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

    lr1110_system_reset( radio );

    HAL_Delay(500);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_Delay(100);
    lr1110_modem_version_t version;
    sprintf( data,"> Reset done!\n\r" );
    CDC_Transmit_FS(&data, strlen(data));
    HAL_Delay( 500 );

    lr1110_bootloader_get_version( radio, &version_bootloader );
    sprintf( data,"Chip in bootloader mode:\n\r" );
    CDC_Transmit_FS(&data, strlen(data));
    HAL_Delay( 500 );
    sprintf( data," - LR1110 TYPE = 0x%02X (0xDF for production)\n\r", version_bootloader.type );
    CDC_Transmit_FS(&data, strlen(data));
    HAL_Delay( 500 );
    sprintf( data," - LR1110 HW   = 0x%02X (0x20 for V2A, 0x21 for V2B, 0x22 for V2C)\n\r", version_bootloader.hw );
    CDC_Transmit_FS(&data, strlen(data));
    HAL_Delay( 500 );

    if( version_bootloader.type == 0xDF )
    {
    	//LED on
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

        sprintf( data,"Start flash erase...\n\r" );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );
        lr1110_bootloader_erase_flash( radio );
        sprintf(data, "> Flash erase done!\n\r" );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );

        lr1110_bootloader_pin_t      pin      = { 0x00 };
        lr1110_bootloader_chip_eui_t chip_eui = { 0x00 };
        lr1110_bootloader_chip_eui_t chip_eui1 = { 0x00 };
        lr1110_bootloader_join_eui_t join_eui = { 0x00 };

        lr1110_bootloader_read_pin( radio, pin );
        lr1110_bootloader_read_chip_eui( radio, chip_eui );
        lr1110_bootloader_read_join_eui( radio, join_eui );

        sprintf( data,"PIN is     0x%02X%02X%02X%02X\n\r", pin[0], pin[1], pin[2], pin[3] );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );
        sprintf( data,"ChipEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n\r", chip_eui[0], chip_eui[1], chip_eui[2], chip_eui[3],
                chip_eui[4], chip_eui[5], chip_eui[6], chip_eui[7] );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );
        sprintf( data,"JoinEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n\r", join_eui[0], join_eui[1], join_eui[2], join_eui[3],
                join_eui[4], join_eui[5], join_eui[6], join_eui[7] );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );
        sprintf( data,"Start flashing firmware...\n\r" );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );

        lr1110_bootloader_write_flash_encrypted_full( radio, 0, buffer, length );
    	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);

        sprintf( data,"> Flashing done!\n\r" );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );

        //LED off
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

        sprintf( data,"Rebooting...\n\r" );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );
        lr1110_bootloader_reboot( radio, false );
        sprintf(data, "> Reboot done!\n\r" );
        CDC_Transmit_FS(&data, strlen(data));
        HAL_Delay( 500 );

        switch( fw_update_direction )
        {
        case LR1110_FIRMWARE_UPDATE_TO_TRX:
        {
            lr1110_system_version_t version_trx = { 0x00 };
            lr1110_system_uid_t     uid         = { 0x00 };

            lr1110_system_get_version( radio, &version_trx );
            sprintf( data,"Chip in transceiver mode:\n\r" );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );
            sprintf(data, " - LR1110 TYPE = 0x%02X\n\r", version_trx.type );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );
            sprintf( data," - LR1110 HW   = 0x%02X\n\r", version_trx.hw );
            CDC_Transmit_FS(&data, strlen(data));
                        HAL_Delay( 500 );
            sprintf( data," - LR1110 FW   = 0x%04X\n\r", version_trx.fw );
            CDC_Transmit_FS(&data, strlen(data));
                        HAL_Delay( 500 );
            lr1110_system_read_uid( radio, uid );

            if( version_trx.fw == fw_expected )
            {
                status = LR1110_FW_UPDATE_OK;
                sprintf(data, "Expected firmware running!\n\r" );
                CDC_Transmit_FS(&data, strlen(data));
                            HAL_Delay( 500 );
                sprintf( data,"Please flash another application (like EVK Demo App).\n\r" );
                CDC_Transmit_FS(&data, strlen(data));
                            HAL_Delay( 500 );
            }
            else
            {
                status = LR1110_FW_UPDATE_ERROR;
                sprintf( data,"Error! Wrong firmware version - please retry.\n\r" );
                CDC_Transmit_FS(&data, strlen(data));
                            HAL_Delay( 500 );
            }
            break;
        }
        case LR1110_FIRMWARE_UPDATE_TO_MODEM:
        {
            lr1110_modem_version_t version_modem = { 0 };

            HAL_Delay(2000);
            lr1110_system_reset( radio );
            while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET);//DIO9 Pin

            lr1110_modem_get_chip_eui( radio, chip_eui1 );

            sprintf( data,"ChipEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n\r", chip_eui1[0], chip_eui1[1], chip_eui1[2], chip_eui1[3],
                            chip_eui1[4], chip_eui1[5], chip_eui1[6], chip_eui1[7] );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );
            lr1110_modem_response_code_t stats = lr1110_modem_get_version( radio, &version_modem );
            sprintf(data, "Chip in modem mode:\n\r" );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );
            sprintf( data," - LR1110 BOOTLOADER = 0x%08x\n\r", version_modem.bootloader );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );
            sprintf( data," - LR1110 FW         = 0x%08x\n\r", version_modem.firmware );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );
            sprintf( data," - LR1110 LORAWAN    = 0x%04x\n\r", version_modem.lorawan );
            CDC_Transmit_FS(&data, strlen(data));
            HAL_Delay( 500 );

            uint32_t fw_version = ( ( uint32_t )( version_modem.functionality ) << 24 ) + version_modem.firmware;

            if( fw_version == fw_expected )
            {
                status = LR1110_FW_UPDATE_OK;
                printf( "Expected firmware running!\n" );
                printf( "Please flash another application (like EVK Demo App).\n" );
                //LED Blink
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_Delay(500);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
            }
            else
            {
                status = LR1110_FW_UPDATE_ERROR;
                printf( "Error! Wrong firmware version - please retry.\n" );
                //LED Blink
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
                HAL_Delay(500);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
            }
            break;
        }
        }
    }
    else
    {
        printf( "Wrong chip type!\n" );

        status = LR1110_FW_UPDATE_WRONG_CHIP_TYPE;
    }

    return status;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
