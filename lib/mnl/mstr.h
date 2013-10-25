#ifndef __MSTR_H__
#define __MSTR_H__

#include "mheads.h"

__BEGIN_DECLS

char* mstr_strip (char *s, char n);

void mstr_rand_string(char *s, int max);
void mstr_rand_string_with_len(char *s, int len);
void mstr_rand_digit_with_len(char *s, int len);

/*
 * same as string_array_split(), but don't allocate memory for each item
 * usually used when 's' contain many many seprator
 * improve 10% performence:
 * 40772ms on fast_split(), 45355 on string_array_split(), per 10000 seprator
 * ****this function will overwrite 's' content****
 */
NEOERR* mstr_array_fast_split(ULIST **list, char *s, const char *sep, int max);


/*
 * make sure charout has inlen*2+1 len
 * hexin: usually uint8_t*, the value is 0~15
 * charout: '0123456789abcdef'
 */
void mstr_hex2str(unsigned char *hexin, unsigned int inlen, unsigned char *charout);

/*
 * make sure charout has inlen/2 len
 * charin: '0123456789abcdef'
 * hexout: usually uint8_t*, the value is 0~15
 */
void mstr_str2hex(unsigned char *charin, unsigned int inlen, unsigned char *hexout);

/* out must be 33 length */
void mstr_md5_buf(unsigned char *in, size_t len, char out[LEN_MD5]);
void mstr_md5_str(char *in, char out[LEN_MD5]);

/* string's utf-8 length */
size_t mstr_ulen(const char *s);
/* string's strlen(), to positon pos */
size_t mstr_upos2len(const char *s, long int pos);

bool mstr_isdigit(char *s);
bool mstr_isdigitn(char *buf, size_t len);
bool mstr_israngen(char *buf, size_t len, int *left, int *right);


void mstr_repchr(char *s, char from, char to);
/*
 * replace string with string
 * return an allocated string, remember to free it
 * mstr_repstr(s, "from1", "to1", "from2", "to2", ...)
 * make sure offer suitable rep_count, or, random errors will occur.
 */
char* mstr_repstr(int rep_count, char *s, ...);
/*
 * replace variable string, around by parameter c, by dst(can be NULL)
 * in:
 * src = $level$, $level$$name$,  name.$level$.in$desc$,  or name.$level$
 * c = $
 * dst foo
 *
 * out:
 *
 * foo, foofoo, name.foo.infoo, or name.foo
 */
char* mstr_repvstr(char *src, char c, char *dst);


/* DJB Hash (left to right, ....abc)*/
unsigned int mstr_hash(const char *str);
/* DJB Hash revert (right to left, abc... )*/
unsigned int mstr_hash_rev(const char *str);


/*
 * more wide rage bin converter
 */
void mstr_bin2char(unsigned char *in, unsigned int inlen, unsigned char *out);

__END_DECLS
#endif    /* __MSTR_H__ */
