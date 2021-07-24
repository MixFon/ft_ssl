#include "../include/sha256.h"

/*
** Заполнение остальных 48 элементов на основе первых 16-ти.
*/
void	mix_data(t_uint *data)
{
	t_uint	elem;
	t_uint	s0;
	t_uint	s1;
	int		i;

	i = 15;
	while (++i < 64)
	{
		elem = data[i - 15];
		s0 = rotate_right(elem, 7) ^ rotate_right(elem, 18) ^ (elem >> 3);
		elem = data[i - 2];
		s1 = rotate_right(elem, 17) ^ rotate_right(elem, 19) ^ (elem >> 10);
		data[i] = data[i - 16] + s0 + data[i - 7] + s1;
	}
}

/*
** Одновление  хеша
*/
void	update_hash(t_sha *sha)
{
	sha->hash[0] += sha->vars[0];
	sha->hash[1] += sha->vars[1];
	sha->hash[2] += sha->vars[2];
	sha->hash[3] += sha->vars[3];
	sha->hash[4] += sha->vars[4];
	sha->hash[5] += sha->vars[5];
	sha->hash[6] += sha->vars[6];
	sha->hash[7] += sha->vars[7];
}

void	assigning_variables(t_sha *sha, t_uint *vars)
{
	sha->vars[H_] = sha->vars[G_];
	sha->vars[G_] = sha->vars[F_];
	sha->vars[F_] = sha->vars[E_];
	sha->vars[E_] = sha->vars[D_] + vars[t1];
	sha->vars[D_] = sha->vars[C_];
	sha->vars[C_] = sha->vars[B_];
	sha->vars[B_] = sha->vars[A_];
	sha->vars[A_] = vars[t1] + vars[t2];
}

/*
** Основной цикл в котором происходит работа над одним 512-ти битным словом.
*/
void	working_words(t_sha *sha, const size_t word)
{
	int		i;
	t_uint	data[256];
	t_uint	vars[6];

	i = -1;
	ft_memset(data, 0, 256);
	ft_memcpy(data, sha->data + word, 64);
	init_vars(sha);
	mix_data(data);
	while (++i < SIZE_SHA256)
	{
		vars[teta0] = rotate_right(sha->vars[A_], 2)
			^ rotate_right(sha->vars[A_], 13) ^ rotate_right(sha->vars[A_], 22);
		vars[ma] = (sha->vars[A_] & sha->vars[B_])
			^ (sha->vars[A_] & sha->vars[C_]) ^ (sha->vars[B_] & sha->vars[C_]);
		vars[t2] = vars[teta0] + vars[ma];
		vars[teta1] = rotate_right(sha->vars[E_], 6)
			^ rotate_right(sha->vars[E_], 11) ^ rotate_right(sha->vars[E_], 25);
		vars[ch] = (sha->vars[E_] & sha->vars[F_])
			^ ((~sha->vars[E_]) & sha->vars[G_]);
		vars[t1] = sha->vars[H_] + vars[teta1]
			+ vars[ch] + sha->arr_k[i] + data[i];
		assigning_variables(sha, vars);
	}
	update_hash(sha);
}

/*
** Разбиение массива данных на слова по 512 бит (64 октета).
*/
void	working_sha256(t_sha *sha)
{
	size_t	word;

	word = 0;
	while (word < sha->count_octets)
	{
		working_words(sha, word);
		word += SIZE_SHA256;
	}
}
