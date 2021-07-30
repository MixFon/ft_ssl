
#ifndef des_h
#define des_h

# include "libft.h"

# define HEX_SYMBOLS "1234567890abcdefABCDEF"

typedef struct s_des
{
	int			flags[9];
	char		*input_file;
	char		*output_file;
	char		*password;
	uint64_t	key;
	uint64_t	salt;
	uint64_t	init_vector;
}				t_des;

enum e_des
{
	des_a,
	des_d,
	des_e,
	des_i,
	des_k,
	des_o,
	des_p,
	des_s,
	des_v
};

void	type_des(int ac, const char **av);

#endif
