
#include "../include/ft_ssl.h"


void	chenge_endian(t_uchar *data, size_t count_octets)
{
	size_t	i;
	t_elem	elem;

	i = 0;
	while (i < count_octets)
	{
		elem.i_elem = *(t_uint *)(data + i);
		(data + i)[0] = elem.c_elem[3];
		(data + i)[1] = elem.c_elem[2];
		(data + i)[2] = elem.c_elem[1];
		(data + i)[3] = elem.c_elem[0];
		i += 4;
	}
}

size_t	get_count_zerors(size_t len)
{
	size_t	count_zeros;
	int		tail;

	count_zeros = 0;
	tail = len % SIZE_MD5;
	if (tail < 56)
		count_zeros = 56 - tail;
	else
		count_zeros = SIZE_MD5 - tail + 56;
	return (count_zeros);
}

/*
** Подготовка входного сообщения
** Добавляет нулевой бит, выравнивает сообщение до кратности 512 бит.
*/
t_uchar	*preparation(const t_uchar *data, size_t *count_octets, t_ssl *ssl)
{
	t_ulong	len_message;
	t_uchar	*temp;
	size_t	count_zeros;
	t_uchar	*zeros;

	len_message = ssl->len_message_oct * 8 % MAX_LEN;
	temp = (t_uchar *)ft_strnew(ssl->len_message_oct + 1);
	ft_memcpy(temp, data, ssl->len_message_oct);
	temp[ssl->len_message_oct] = 0x80;
	count_zeros = get_count_zerors(ssl->len_message_oct);
	*count_octets = ssl->len_message_oct + count_zeros + 8;
	zeros = (t_uchar *)ft_strnew(*count_octets);
	ft_memcpy(zeros, temp, ssl->len_message_oct + 1);
	ft_strdel((char **)&temp);
	if (!ft_strcmp(ssl->alg->name, "md5"))
		ft_memcpy(zeros + *count_octets - 8, &len_message, 8);
	else
	{
		chenge_endian(zeros, *count_octets);
		*(t_ulong *)(zeros + *count_octets - 4) = len_message;
	}
	return (zeros);
}

/*
** Возвращает хеш в зависимости от алгоритма хеширования.
*/
t_uchar	*get_hash(const t_uchar *data, t_ssl *ssl)
{
	t_uchar	*hash;
	size_t	count_octets;
	t_uchar	*new_data;

	new_data = preparation(data, &count_octets, ssl);
	hash = ssl->alg->algorithm(new_data, count_octets);
	free(new_data);
	return (hash);
}

void	print_hash(t_ssl *ssl, const t_uchar *data)
{
	int	i;

	i = -1;
	while (++i < ssl->alg->size)
		ft_printf("%02x", data[i]);
	ft_putendl("");
}
