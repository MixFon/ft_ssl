#ifndef BASE64_H
#define BASE64_H

# include "libft.h"
# include "ft_ssl.h"

# define UNKNOWN_SYMBOL 0

typedef struct	s_base64
{
	int			flags[4];
	char		*chiphertext;
	char		*input_file;
	char	 	*output_file;
	char 		*symbols;
	size_t		size_chiphertext;
}				t_base64;

enum e_flags
{
	base_e,
	base_d,
	base_i,
	base_o
};

void	type_base64(int ac, const char **av);
void	decoding(t_base64 *base, char *data, size_t size);
void	encoding(t_base64 *base, char *data, size_t size);
void	init_base64(t_base64 *base);
void	deinit_base64(t_base64 *base);



#endif
