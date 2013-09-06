#include "mheads.h"

int mutil_compare_int(const void *a, const void *b)
{
    int *i = (int*)a;
    int *j = (int*)b;
    return *i-*j;
}

int mutil_compare_inta(const void *a, const void *b)
{
    int *i = (int*)a;
    char *j = (char*)b;
    
    return *i - atoi(j);
}

void mutil_rand_numbers_byarray(int in[], int inlen, int out[], int outlen)
{
    int pos;

    if (outlen > inlen) return;
    
    for (int i = 0; i < outlen; i++) {
        pos = neo_rand(inlen-i-1);
        out[i] = in[pos];
        
        in[pos] = in[inlen-i-1];
    }
}

void mutil_rand_numbers(int max, int out[], int outlen)
{
    if (outlen > max) return;

    int in[max];

    for (int i = 0; i < max; i++) in[i] = i+1;

    return mutil_rand_numbers_byarray(in, max, out, outlen);
}

int mutil_satoi(const char *nptr)
{
    if (!nptr) return 0;

    return atoi(nptr);
}
