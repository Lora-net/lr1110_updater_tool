/*!
 * \file      gui.c
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

#include "semtech_logo.h"
#include "stdio.h"

static lv_obj_t* screen;
static lv_obj_t* icon;
static lv_obj_t* lbl_title;
static lv_obj_t* lbl_version;
static lv_obj_t* preload;
static lv_obj_t* lbl_status;

static lv_style_t screen_style;
static lv_style_t title_style;
static lv_style_t preloader_style;

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

void gui_init( uint32_t fw_expected )
{
    char buffer[30] = { 0 };

    lv_style_copy( &( screen_style ), &lv_style_scr );
    screen_style.body.main_color = LV_COLOR_BLACK;
    screen_style.body.grad_color = LV_COLOR_BLACK;
    screen_style.text.color      = LV_COLOR_WHITE;

    lv_style_copy( &( title_style ), &lv_style_scr );
    title_style.body.main_color = LV_COLOR_BLACK;
    title_style.body.grad_color = LV_COLOR_BLACK;
    title_style.text.color      = LV_COLOR_WHITE;
    title_style.text.font       = &lv_font_roboto_22;

    screen = lv_obj_create( NULL, NULL );
    lv_obj_set_style( screen, &( screen_style ) );

    icon = lv_img_create( screen, NULL );
    lv_obj_align( icon, NULL, LV_ALIGN_IN_TOP_LEFT, 17, 0 );
    lv_img_set_src( icon, &semtech_logo );

    lv_obj_t* lbl_title = lv_label_create( screen, NULL );
    lv_obj_set_style( lbl_title, &( title_style ) );
    lv_label_set_long_mode( lbl_title, LV_LABEL_LONG_BREAK );
    lv_label_set_align( lbl_title, LV_LABEL_ALIGN_CENTER );
    lv_label_set_text( lbl_title, "LR1110\nUPDATER TOOL" );
    lv_obj_set_width( lbl_title, 240 );
    lv_obj_align( lbl_title, NULL, LV_ALIGN_CENTER, 0, -30 );

    sprintf( buffer, "Update to firmware 0x%08x", fw_expected );

    lv_obj_t* lbl_fw = lv_label_create( screen, NULL );
    lv_obj_set_style( lbl_fw, &( screen_style ) );
    lv_label_set_long_mode( lbl_fw, LV_LABEL_LONG_BREAK );
    lv_label_set_align( lbl_fw, LV_LABEL_ALIGN_CENTER );
    lv_label_set_text( lbl_fw, buffer );
    lv_obj_set_width( lbl_fw, 240 );
    lv_obj_align( lbl_fw, NULL, LV_ALIGN_CENTER, 0, 20 );

    lbl_status = lv_label_create( screen, NULL );
    lv_obj_set_style( lbl_status, &( screen_style ) );
    lv_label_set_long_mode( lbl_status, LV_LABEL_LONG_BREAK );
    lv_label_set_align( lbl_status, LV_LABEL_ALIGN_CENTER );
    lv_label_set_text( lbl_status, "UPDATE ON GOING..." );
    lv_obj_set_width( lbl_status, 240 );
    lv_obj_align( lbl_status, NULL, LV_ALIGN_CENTER, 0, 80 );

    lv_scr_load( screen );
}

void gui_update( const char* txt ) { lv_label_set_text( lbl_status, txt ); }

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
