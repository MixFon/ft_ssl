#ifndef SHA256_H
# define SHA256_H

# include "stdio.h"
# include "libft.h"

# define SIZE_SHA256		64
# define LEN_HASH_SHA256	32

# define A_					0
# define B_					1
# define C_					2
# define D_					3
# define E_					4
# define F_					5
# define G_					6
# define H_					7

typedef unsigned int		t_uint;
typedef unsigned char		t_uchar;
typedef unsigned long long	t_ulong;

typedef struct s_sha
{
	const t_uchar	*data;
	t_uint			arr_k[SIZE_SHA256];
	t_uint			arr_s[SIZE_SHA256];
	size_t			count_octets;
	t_uint			hash[8];
	t_uint			vars[8];
}					t_sha;

enum e_var
{
	teta0,
	teta1,
	ch,
	ma,
	t1,
	t2
};

void	chenge_endian(t_uchar *data, size_t count_octets);
void	print_bits(t_uchar *str, size_t len);
t_uchar	*get_string_hash(t_uint *hash, int len);
t_uchar	*alg_sha256(const t_uchar *data, const size_t count_octets);

#endif
/*
** File init_arr_k_sha256.c
*/
void	init_arr_k_sha256(t_sha *sha);
void	init_hash_start_value(t_sha *sha);
void	init_sha256(t_sha *sha, const t_uchar *data, const size_t count_octets);
void	init_vars(t_sha *sha);
t_uint	rotate_right(t_uint elem, int count);
/*
** File mix_data.c
*/
void	mix_data(t_uint *data);
void	update_hash(t_sha *sha);
void	assigning_variables(t_sha *sha, t_uint *vars);
void	working_words(t_sha *sha, const size_t word);
void	working_sha256(t_sha *sha);
/*
** File alg_sha256.c
*/
t_uchar	*alg_sha256(const t_uchar *data, const size_t count_octets);
