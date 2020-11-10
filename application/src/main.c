/*!
 * \file      main.c
 *
 * \brief     LR1110 updater tool application entry point
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

#include "lr1110_modem_1.0.7.h"

#include "configuration.h"
#include "system.h"
#include "stdio.h"
#include "string.h"
#include "lr1110_firmware_update.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "gui.h"

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

radio_t radio = {
    SPI1,
    { LR1110_NSS_PORT, LR1110_NSS_PIN },
    { LR1110_RESET_PORT, LR1110_RESET_PIN },
    { LR1110_IRQ_PORT, LR1110_IRQ_PIN },
    { LR1110_BUSY_PORT, LR1110_BUSY_PIN },
};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

int main( void )
{
    lr1110_fw_update_status_t status     = LR1110_FW_UPDATE_ERROR;
    bool                      is_updated = false;

    system_init( );

    system_time_wait_ms( 2000 );

    lv_init( );
    lv_port_disp_init( );

    printf( "LR1110 updater tool v1.1.0\n" );

    gui_init( LR1110_FIRMWARE_VERSION );

    if( LR1110_FIRMWARE_UPDATE_TO == LR1110_FIRMWARE_UPDATE_TO_TRX )
    {
        printf( "Update to transceiver firmware 0x%04x\n", LR1110_FIRMWARE_VERSION );
    }
    else
    {
        printf( "Update to modem firmware 0x%04x\n", LR1110_FIRMWARE_VERSION );
    }

    while( 1 )
    {
        lv_task_handler( );

        if( is_updated == false )
        {
            status = lr1110_update_firmware( &radio, LR1110_FIRMWARE_UPDATE_TO, LR1110_FIRMWARE_VERSION,
                                             lr1110_firmware_image, ( uint32_t ) LR1110_FIRMWARE_IMAGE_SIZE );

            switch( status )
            {
            case LR1110_FW_UPDATE_OK:
                gui_update( "UPDATE DONE!\nPlease flash another application\n(like EVK Demo App)" );
                break;
            case LR1110_FW_UPDATE_WRONG_CHIP_TYPE:
                gui_update( "WRONG CHIP TYPE" );
                break;
            case LR1110_FW_UPDATE_ERROR:
                gui_update( "ERROR\nWrong firmware version\nPlease retry" );
                break;
            }

            is_updated = true;
        }
    };
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
