#ifndef FT_SSL_H
#define FT_SSL_H

#include "md5.h"

# define P          0
# define Q          1
# define R          2
# define S          3

typedef struct  s_ssl
{
    int         flags[4];
    char        *string;
    char        **file_names;
    t_uchar     *hash;
}               t_ssl;


#endif /* ft_ssl_h */
