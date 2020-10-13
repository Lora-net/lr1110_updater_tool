#ifndef __SYSTEM_TIME__
#define __SYSTEM_TIME__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     system_time_init( void );
void     system_time_wait_ms( uint32_t time_in_ms );
void     system_time_IncreaseTicker( void );
uint32_t system_time_GetTicker( void );

#ifdef __cplusplus
}
#endif

#endif  // __SYSTEM_TIME__
