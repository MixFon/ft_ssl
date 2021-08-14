#include "../include/hmac.h"

#include "hmac.h"

t_uchar	*get_hash_md5(const char *data, size_t size)
{
	t_uchar	*hash;
	size_t	count_octets;
	t_uchar	*new_data;
	t_ssl	ssl;

	ssl.len_message_oct = size;
	ssl.alg = &(get_algorithms())[0];
	new_data = preparation((const t_uchar *)data, &count_octets, &ssl);
	hash = alg_md5(new_data, count_octets);
	free(new_data);
	return (hash);
}

/*
** Возвращет К0 на основе размера ключа.
*/
t_uchar	*get_key_0(const char *key)
{
	t_uchar	*rezult;
	size_t	len;

	len = ft_strlen(key);
	if (len > SIZE_MD5)
		return (get_hash_md5(key, ft_strlen(key)));
	else
	{
		rezult = (t_uchar *)ft_strnew(SIZE_MD5 + 1);
		ft_memcpy(rezult, key, len);
		return (rezult);
	}
	return (NULL);
}

void	get_keypad(t_uchar *key_0, t_uchar *pad, int number)
{
	int		i;

	i = -1;
	ft_memset(pad, number, SIZE_MD5);
	while (++i < SIZE_MD5)
		pad[i] = key_0[i] ^ pad[i];
}

t_uchar	*concat_two_string(t_uchar *one, size_t size_one,
						   t_uchar *two, size_t size_two)
{
	t_uchar	*rezult;

	rezult = (t_uchar *)ft_strnew(size_one + size_two);
	ft_memcpy(rezult, one, size_one);
	ft_memcpy(rezult + size_one, two, size_two);
	return (rezult);
}

t_uchar	*get_hmac(t_uchar *ikeypad, t_uchar *okeypad, const t_uchar *salt)
{
	t_uchar	*hmac;
	t_uchar	*cat;
	size_t	size;

	size = ft_strlen((char *)salt);
	cat = concat_two_string(ikeypad, SIZE_MD5, (t_uchar *)salt, size);
	hmac = get_hash_md5((const char *)cat, SIZE_MD5 + size);
	free(cat);
	cat = concat_two_string(okeypad, SIZE_MD5, hmac, 16);
	free(hmac);
	hmac = get_hash_md5((const char *)cat, SIZE_MD5 + 16);
	free(cat);
	return (hmac);
}
