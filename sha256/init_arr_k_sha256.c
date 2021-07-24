#include "../include/sha256.h"
#include "sha256.h"

void	init_arr_k_sha256(t_sha *sha)
{
	static t_uint	temp[] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

	ft_memcpy(sha->arr_k, temp, sizeof(t_uint) * SIZE_SHA256);
}

/*
** Заполнение начального вектора.
*/
void	init_hash_start_value(t_sha *sha)
{
	sha->hash[0] = 0x6A09E667;
	sha->hash[1] = 0xBB67AE85;
	sha->hash[2] = 0x3C6EF372;
	sha->hash[3] = 0xA54FF53A;
	sha->hash[4] = 0x510E527F;
	sha->hash[5] = 0x9B05688C;
	sha->hash[6] = 0x1F83D9AB;
	sha->hash[7] = 0x5BE0CD19;
}

/*
** Инициализация структуры.
*/
void	init_sha256(t_sha *sha, const t_uchar *data, const size_t count_octets)
{
	ft_memset(sha, 0, sizeof(t_sha));
	sha->data = data;
	sha->count_octets = count_octets;
	init_hash_start_value(sha);
	init_arr_k_sha256(sha);
}

/*
** Инициализация временных переменных стартовыми значениями.
*/
void	init_vars(t_sha *sha)
{
	sha->vars[0] = sha->hash[0];
	sha->vars[1] = sha->hash[1];
	sha->vars[2] = sha->hash[2];
	sha->vars[3] = sha->hash[3];
	sha->vars[4] = sha->hash[4];
	sha->vars[5] = sha->hash[5];
	sha->vars[6] = sha->hash[6];
	sha->vars[7] = sha->hash[7];
}

/*
** Циклический сдвиг вправо.
*/
t_uint	rotate_right(t_uint elem, int count)
{
	return ((elem >> count) | (elem << (32 - count)));
}
