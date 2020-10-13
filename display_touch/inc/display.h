/*!
 * \file      display.h
 *
 * \brief
 *
 * \copyright Copyright Semtech Corporation 2019. All rights reserved.
 *
 * Disclaimer: “The Semtech software and the documentation are provided "as is"
 * without any express or implied warranty of any kind from Semtech or from any
 * other person or entity, including warranties of merchantability,
 * noninfringement, or fitness for a particular purpose.”
 *
 * \author    Semtech WSP Applications Team
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

void display_init( void );

void display_send_command( const uint8_t command );
void display_send_data( const uint16_t data );

#ifdef __cplusplus
}
#endif

#endif
