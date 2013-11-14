#ifndef __MUTIL_H__
#define __MUTIL_H__

#include "mheads.h"

__BEGIN_DECLS

int  mutil_compare_int(const void *a, const void *b);
int  mutil_compare_inta(const void *a, const void *b);

/*
 * get 'outlen' random different(unrepeat) numbers, from 'in' with 'inlen' length,
 * make sure 'out' have 'outlen' length
 */
void mutil_rand_numbers_byarray(int in[], int inlen, int out[], int outlen);
/*
 * get 'outlen' random different(unrepeat) numbers, with maxnumber 'max'
 * If you need zero, please -1 for every out[i]
 * don't pass a huge max, because it'll use many memory
 * make sure 'out' have 'outlen' length
 */
void mutil_rand_numbers(int max, int out[], int outlen);

/*
 * safe atoi
 */
int mutil_satoi(const char *nptr);

/*
 * res must be char xxx[LEN_TM]
 */
bool   mutil_getdatetime(char *res, int len, const char *fmt, time_t second);
/*
 * res must be char xxx[LEN_TM_GMT]
 */
bool   mutil_getdatetime_gmt(char *res, int len, const char *fmt, time_t second);
time_t mutil_get_abssec(char *fmt, char *time);
void   mutil_utc_time(struct timespec *ts);


void mutil_dump_matrix(char *prefix);
void mutil_create_earth(double R, double H, double K, double Z);
void mutil_rend_earth(GLuint texid);

__END_DECLS
#endif    /* __MUTIL_H__ */
