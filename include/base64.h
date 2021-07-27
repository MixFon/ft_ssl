#ifndef BASE64_H
#define BASE64_H

# include "libft.h"
# include "ft_ssl.h"

typedef struct	s_base64
{
	int			flags[4];
	char		*ciphertext;
	char		*input_file;
	char	 	*output_file;
	char 		*symbols;
	size_t		index;
}				t_base64;

enum e_flags
{
	base_e,
	base_d,
	base_i,
	base_o
};

void type_base64(int ac, const char **av);

#endif
