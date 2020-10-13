#include "system_uart.h"

int stdout_init( void ) { return ( 0 ); }

int stdout_putchar( int ch ) { return system_uart_send_char( ch ); }

int stdout_getchar( void ) { return system_uart_receive_char( ); }
