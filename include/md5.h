#ifndef MD5_H
# define MD5_H

# include "stdio.h"
# include "libft.h"

# define SIZE_MD5	   64
# define LEN_HASH_MD5   16

# define A		  0
# define B		  1
# define C		  2
# define D		  3
# define F		  4
# define G		  5

typedef unsigned int		t_uint;
typedef unsigned char		t_uchar;
typedef unsigned long long	t_ulong;

typedef struct s_md
{
	const t_uchar	*data;
	t_uint			arr_k[SIZE_MD5];
	t_uint			arr_s[SIZE_MD5];
	size_t			count_octets;
	t_uint			hash[4];
	t_uint			vars[6];
	t_uint			f;
	void			(*stages[4])(t_uint *vars, int i);
}					t_md;

void	print_bits(t_uchar *str, size_t len);
t_uchar	*alg_md5(const t_uchar *data, size_t count_octets);

#endif
/*
** File fill_arr_k.c
*/
void	fill_arr_k(t_md *md);
void	fill_arr_s(t_md *md);
void	init(t_md *md, const t_uchar *data, size_t count_octets);
void	print_bits(t_uchar *str, size_t len);
void	step_three(t_md *md);
/*
** File stage_one.c
*/
void	stage_one(t_uint *vars, int i);
void	stage_two(t_uint *vars, int i);
void	stage_three(t_uint *vars, int i);
void	stage_four(t_uint *vars, int i);
void	init_start_vars(t_md *md);
/*
** File rounds.c
*/
void	rounds(t_md *md, const int words);
void	step_four(t_md *md);
t_uchar	*get_string_hash(t_uint *hash, int len);
t_uchar	*alg_md5(const t_uchar *data, size_t count_octets);
