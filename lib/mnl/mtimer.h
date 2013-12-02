/*
 * A simple timer for measuring delays.
 * Alberto Bertogli (albertito@blitiri.com.ar) - September/2006
 *
 * Use it like this:
 * 1. trace the time message
 *     mtimer_start();
 *     ... [code] ...
 *     mtimer_stop("useconds for load file  %s", fname);
 * 2. use the splaped on your hand
 *     mtimer_start();
 *     ... [code] ...
 *     unsigned long usec = mtimer_stop(NULL);
 *
 * Nested timers are supported.
 * mtimer_start() and mtimer_stop() must be appeared in pair
 *
 * 1000000 usecs == 1 sec
 */

#ifndef __MTIMER_H__
#define __MTIMER_H__

#include "mheads.h"

__BEGIN_DECLS

#define TIMER_NUM 64
#define TIMER_MSG_LEN 256

void          mtimer_start(void);
unsigned long mtimer_stop(char *fmt, ...)
                          ATTRIBUTE_PRINTF(1, 2);

uint32_t mtimer_tick_callback(uint32_t interval, void *param);

__END_DECLS
#endif    /* __MTIMER_H__ */
