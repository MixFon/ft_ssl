#include "../include/md5.h"

/*
** Основное тело алгоритма. Используются указатели на функцию.
*/
void	rounds(t_md *md, const int words)
{
	int		i;
	t_uint	*data;

	i = -1;
	data = (t_uint *)(md->data + words);
	init_start_vars(md);
	while (++i < SIZE_MD5)
	{
		md->stages[i / LEN_HASH_MD5](md->vars, i);
		md->vars[F] = md->vars[F] + md->vars[A] + md->arr_k[i]
			+ data[md->vars[G]];
		md->vars[A] = md->vars[D];
		md->vars[D] = md->vars[C];
		md->vars[C] = md->vars[B];
		md->vars[B] = md->vars[B] + ((md->vars[F] << md->arr_s[i])
				| (md->vars[F] >> (32 - md->arr_s[i])));
	}
	md->hash[A] += md->vars[A];
	md->hash[B] += md->vars[B];
	md->hash[C] += md->vars[C];
	md->hash[D] += md->vars[D];
}

/*
** Вычисления в цикле.
*/
void	step_four(t_md *md)
{
	size_t	words;

	words = 0;
	md->stages[0] = stage_one;
	md->stages[1] = stage_two;
	md->stages[2] = stage_three;
	md->stages[3] = stage_four;
	while (words < md->count_octets)
	{
		rounds(md, words);
		words += SIZE_MD5;
	}
}

/*
** Создает строку - hash. Память выделяется динамически.
*/
t_uchar	*get_string_hash(t_uint *hash, int len)
{
	t_uchar	*result;
	int		i;

	i = -1;
	result = (t_uchar *)ft_strnew(len + 1);
	while (++i < len)
		result[i] = ((t_uchar *)hash)[i];
	return (result);
}

t_uchar	*alg_md5(const t_uchar *data, size_t count_octets)
{
	t_uchar	*result;
	t_md	md;

	init(&md, data, count_octets);
	step_three(&md);
	step_four(&md);
	result = get_string_hash(md.hash, LEN_HASH_MD5);
	return (result);
}
