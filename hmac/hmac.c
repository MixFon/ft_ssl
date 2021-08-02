#include "hmac.h"

t_uchar	*get_hash_md5(const char *data)
{
	t_uchar	*hash;
	size_t	count_octets;
	t_uchar	*new_data;
	t_ssl	ssl;

	ssl.len_message_oct = ft_strlen(data);
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
		return (get_hash_md5(key));
	else
	{
		rezult = (t_uchar *)ft_strnew(SIZE_MD5);
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
	pad[i] = '\0';
}

t_uchar	*get_hmac(t_uchar *ikeypad, t_uchar *okeypad, const char *salt)
{
	t_uchar	*hmac;
	t_uchar	*cat;

	cat = (t_uchar *)ft_multi_strdup(2, ikeypad, salt);
	hmac = get_hash_md5((const char *)cat);
	free(cat);
	cat = (t_uchar *)ft_multi_strdup(2, okeypad, hmac);
	free(hmac);
	hmac = get_hash_md5((const char *)cat);
	free(cat);
	return (hmac);
}

t_uchar *hmac_md5(const char *key, const char *salt)
{
	t_uchar	*key_0;
	t_uchar	ikeypad[SIZE_MD5 + 1];
	t_uchar	okeypad[SIZE_MD5 + 1];

	key_0 = get_key_0(key);
	get_keypad(key_0, ikeypad, 0x36);
	get_keypad(key_0, okeypad, 0x5c);
	free(key_0);
	return (get_hmac(ikeypad, okeypad, salt));
}
