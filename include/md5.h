
#ifndef MD5_H
#define MD5_H

#include "stdio.h"
#include "libft.h"

# define SIZE_MD5       64
# define LEN_HASH_MD5   16

# define A          0
# define B          1
# define C          2
# define D          3
# define F          4
# define G          5

# define ABS(x) ((x)<0?-(x):(x))

typedef unsigned int        t_uint;
typedef unsigned char       t_uchar;
typedef unsigned long long  t_ulong;

typedef struct      s_md
{
    const t_uchar   *data;
    t_uint          arr_k[SIZE_MD5];
    t_uint          arr_s[SIZE_MD5];
    size_t          count_octets;
    t_uint          hash[4];
    t_uint          vars[6];
    t_uint          f;
    void            (*stages[4])(t_uint *vars, int i);
}                   t_md;

void    print_bits(t_uchar *str, size_t len);
t_uchar *alg_md5(const t_uchar *data, size_t count_octets);

#endif
