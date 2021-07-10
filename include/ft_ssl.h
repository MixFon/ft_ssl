#ifndef FT_SSL_H
#define FT_SSL_H

#include <sys/stat.h>
#include "md5.h"

# define P          0
# define Q          1
# define R          2
# define S          3

# define BUFF       4096

typedef struct  s_ssl
{
    int         flags[4];
    char        **strings;
    size_t      count_strings;
    char        **file_names;
    size_t      count_file_names;
    t_uchar     *hash;
    char        *name_algorithm;
}               t_ssl;


#endif /* ft_ssl_h */
