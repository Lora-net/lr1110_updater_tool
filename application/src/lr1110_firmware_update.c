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
#include "system.h"
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

static gpio_t lr1110_busy     = { LR1110_BUSY_PORT, LR1110_BUSY_PIN };
static gpio_t lr1110_led_tx   = { LR1110_LED_TX_PORT, LR1110_LED_TX_PIN };
static gpio_t lr1110_led_rx   = { LR1110_LED_RX_PORT, LR1110_LED_RX_PIN };
static gpio_t lr1110_led_scan = { LR1110_LED_SCAN_PORT, LR1110_LED_SCAN_PIN };

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

    lr1110_system_stat1_t stat1;
    lr1110_system_stat2_t stat2;
    uint32_t              irq;

    printf( "Reset the chip...\n" );

    system_gpio_init_direction_state( lr1110_busy, SYSTEM_GPIO_PIN_DIRECTION_OUTPUT, SYSTEM_GPIO_PIN_STATE_LOW );

    lr1110_system_reset( radio );

    system_time_wait_ms( 500 );
    system_gpio_init_direction_state( lr1110_busy, SYSTEM_GPIO_PIN_DIRECTION_INPUT, SYSTEM_GPIO_PIN_STATE_LOW );
    system_time_wait_ms( 100 );

    printf( "> Reset done!\n" );

    lr1110_bootloader_get_version( radio, &version_bootloader );
    printf( "Chip in bootloader mode:\n" );
    printf( " - LR1110 TYPE = 0x%02X (0xDF for production)\n", version_bootloader.type );
    printf( " - LR1110 HW   = 0x%02X (0x20 for V2A, 0x21 for V2B, 0x22 for V2C)\n", version_bootloader.hw );

    if( version_bootloader.type == 0xDF )
    {
        system_gpio_set_pin_state( lr1110_led_scan, SYSTEM_GPIO_PIN_STATE_HIGH );

        printf( "Start flash erase...\n" );
        lr1110_bootloader_erase_flash( radio );
        printf( "> Flash erase done!\n" );

        lr1110_bootloader_pin_t      pin      = { 0x00 };
        lr1110_bootloader_chip_eui_t chip_eui = { 0x00 };
        lr1110_bootloader_join_eui_t join_eui = { 0x00 };

        lr1110_bootloader_read_pin( radio, pin );
        lr1110_bootloader_read_chip_eui( radio, chip_eui );
        lr1110_bootloader_read_join_eui( radio, join_eui );

        printf( "PIN is     0x%02X%02X%02X%02X\n", pin[0], pin[1], pin[2], pin[3] );
        printf( "ChipEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n", chip_eui[0], chip_eui[1], chip_eui[2], chip_eui[3],
                chip_eui[4], chip_eui[5], chip_eui[6], chip_eui[7] );
        printf( "JoinEUI is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n", join_eui[0], join_eui[1], join_eui[2], join_eui[3],
                join_eui[4], join_eui[5], join_eui[6], join_eui[7] );
        printf( "Start flashing firmware...\n" );

        lr1110_bootloader_write_flash_encrypted_full( radio, 0, buffer, length );

        printf( "> Flashing done!\n" );

        system_gpio_set_pin_state( lr1110_led_scan, SYSTEM_GPIO_PIN_STATE_LOW );

        printf( "Rebooting...\n" );
        lr1110_bootloader_reboot( radio, false );
        printf( "> Reboot done!\n" );

        switch( fw_update_direction )
        {
        case LR1110_FIRMWARE_UPDATE_TO_TRX:
        {
            lr1110_system_version_t version_trx = { 0x00 };
            lr1110_system_uid_t     uid         = { 0x00 };

            lr1110_system_get_version( radio, &version_trx );
            printf( "Chip in transceiver mode:\n" );
            printf( " - LR1110 TYPE = 0x%02X\n", version_trx.type );
            printf( " - LR1110 HW   = 0x%02X\n", version_trx.hw );
            printf( " - LR1110 FW   = 0x%04X\n", version_trx.fw );

            lr1110_system_read_uid( radio, uid );

            if( version_trx.fw == fw_expected )
            {
                status = LR1110_FW_UPDATE_OK;
                printf( "Expected firmware running!\n" );
                printf( "Please flash another application (like EVK Demo App).\n" );
                system_gpio_set_pin_state( lr1110_led_rx, SYSTEM_GPIO_PIN_STATE_HIGH );
            }
            else
            {
                status = LR1110_FW_UPDATE_ERROR;
                printf( "Error! Wrong firmware version - please retry.\n" );
                system_gpio_set_pin_state( lr1110_led_tx, SYSTEM_GPIO_PIN_STATE_HIGH );
            }
            break;
        }
        case LR1110_FIRMWARE_UPDATE_TO_MODEM:
        {
            lr1110_modem_version_t version_modem = { 0 };

            system_time_wait_ms( 2000 );

            lr1110_modem_get_version( radio, &version_modem );
            printf( "Chip in modem mode:\n" );
            printf( " - LR1110 BOOTLOADER = 0x%08x\n", version_modem.bootloader );
            printf( " - LR1110 FW         = 0x%08x\n", version_modem.firmware );
            printf( " - LR1110 LORAWAN    = 0x%04x\n", version_modem.lorawan );

            uint32_t fw_version = ( ( uint32_t )( version_modem.functionality ) << 24 ) + version_modem.firmware;

            if( fw_version == fw_expected )
            {
                status = LR1110_FW_UPDATE_OK;
                printf( "Expected firmware running!\n" );
                printf( "Please flash another application (like EVK Demo App).\n" );
                system_gpio_set_pin_state( lr1110_led_rx, SYSTEM_GPIO_PIN_STATE_HIGH );
            }
            else
            {
                status = LR1110_FW_UPDATE_ERROR;
                printf( "Error! Wrong firmware version - please retry.\n" );
                system_gpio_set_pin_state( lr1110_led_tx, SYSTEM_GPIO_PIN_STATE_HIGH );
            }
            break;
        }
        }
    }
    else
    {
        printf( "Wrong chip type!\n" );
        system_gpio_set_pin_state( lr1110_led_tx, SYSTEM_GPIO_PIN_STATE_HIGH );
        status = LR1110_FW_UPDATE_WRONG_CHIP_TYPE;
    }

    return status;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
