#ifndef DES_H
# define DES_H

# include "libft.h"
# include "ft_ssl.h"

# define HEX_SYMBOLS "1234567890abcdefABCDEF"

# define KDLEN 8

typedef struct s_des
{
	int			flags[10];
	char		*input_file;
	char		*output_file;
	char		*password;
	uint8_t		*message;
	size_t		size_message;
	uint64_t	key;
	uint64_t	key56;
	uint64_t	keys[16];
	uint64_t	salt;
	uint64_t	init_vector;
	uint32_t	c0;
	uint32_t	d0;
}				t_des;

typedef union u_block
{
	uint64_t	block64;
	uint32_t	blocks32[2];
	uint8_t		blocks8[8];
}				t_block;

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
	des_v,
	des_w
};

void	type_des(int ac, const char **av);

#endif
