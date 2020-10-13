#include "system_time.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"

volatile static uint32_t ticker = 0;

void system_time_init( void ) { LL_SYSTICK_EnableIT( ); }

void system_time_wait_ms( uint32_t time_in_ms ) { LL_mDelay( time_in_ms ); }

void system_time_IncreaseTicker( void ) { ticker++; }

uint32_t system_time_GetTicker( void ) { return ticker; }
