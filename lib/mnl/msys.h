#ifndef __MSYS_H__
#define __MSYS_H__

#include "mheads.h"

__BEGIN_DECLS

void msys_makesure_coredump();
int  msys_systemf(char *fmt, ...)
                  ATTRIBUTE_PRINTF(1, 2);
int  msys_execvf(char *argv[], char *fmt, ...)
                 ATTRIBUTE_PRINTF(2, 3);


/* res must be char xxx[LEN_TM] */
bool   msys_getdatetime(char *res, int len, const char *fmt, time_t second);
/* res must be char xxx[LEN_TM_GMT] */
bool   msys_getdatetime_gmt(char *res, int len, const char *fmt, time_t second);
time_t msys_get_abssec(char *fmt, char *time);
void   msys_utc_time(struct timespec *ts);

__END_DECLS
#endif  /* __MSYS_H__ */
