#ifndef FT_SSL_H
#define FT_SSL_H

#include <sys/stat.h>
#include "md5.h"
#include "sha256.h"
#include "libft.h"

# define UINT_MAX   0xFFFFFFFF
# define MAX_LEN    0xFFFFFFFFFFFFFFFF

# define P          0
# define Q          1
# define R          2
# define S          3

# define BUFF       4096

typedef struct  s_alg
{
	t_uchar	*(*algorithm)(const t_uchar *data, size_t count_octets);
	char	*name;
	char	*name_case;
	int		size;
}				t_alg;

typedef struct  s_ssl
{
    int         flags[4];
    char        **strings;
    size_t      count_strings;
    char        **file_names;
    size_t      count_file_names;
    t_uchar     *hash;
    //char        *name_algorithm;
    size_t      len_message_oct;
	int			i;
	int			k;
	t_alg		*alg;
}               t_ssl;

typedef union       u_elem
{
    unsigned int    i_elem;
    unsigned char   c_elem[4];
}                   t_elem;

t_alg	*get_algorithms(void);
//t_uchar *alg_md5(const t_uchar *data, size_t count_octets)

#endif /* ft_ssl_h */
