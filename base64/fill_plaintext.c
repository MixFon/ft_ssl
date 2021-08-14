#include "../include/base64.h"

/*
** Заполнение шифратекста.
*/
void	fill_plaintext(t_base64 *base, char *numbers)
{
	int	i;

	i = -1;
	while (++i < 3)
		base->chiphertext[base->size_chiphertext + i] = numbers[i];
	base->size_chiphertext += 3;
}

/*
** Кодирование информации.
*/
void	encoding(t_base64 *base, char *data, size_t size)
{
	size_t	i;
	char	*word;
	uint8_t	numbers[4];
	int		equal;

	i = 0;
	equal = 0;
	if (size % 3 != 0)
		equal = 3 - (size % 3);
	base->chiphertext = ft_strnew(size + size / 3 + 3);
	resize_data(&data, &size);
	while (i < size)
	{
		word = (data + i);
		numbers[0] = (word[0] & 0xff) >> 2;
		numbers[1] = ((word[0] & 0x3) << 4) | ((word[1] & 0xff) >> 4);
		numbers[2] = ((word[1] & 0xf) << 2) | ((word[2] & 0xff) >> 6);
		numbers[3] = word[2] & 0x3f;
		fill_ciphertext(base, numbers);
		i += 3;
	}
	while (equal > 0)
		base->chiphertext[size + size / 3 - equal--] = '=';
	free(data);
}

/*
** Возвращает индекс в массиве символов base64.
*/
int	get_index(t_base64 *base, char c)
{
	int	i;

	i = -1;
	while (++i < 64)
	{
		if (base->symbols[i] == c)
			return (i);
	}
	return (UNKNOWN_SYMBOL);
}

/*
** Расшифровка данных.
*/
void	decoding(t_base64 *base, char *data, size_t size)
{
	size_t	i;
	char	*word;
	char	indexes[3];

	i = 0;
	base->chiphertext = ft_strnew(size);
	while (i < size)
	{
		word = data + i;
		indexes[0] = (get_index(base, word[0]) << 2)
			| ((get_index(base, word[1]) & 0x30) >> 4);
		indexes[1] = (get_index(base, word[1]) << 4)
			| ((get_index(base, word[2]) & 0x3c) >> 2);
		indexes[2] = (get_index(base, word[2]) << 6) | get_index(base, word[3]);
		fill_plaintext(base, indexes);
		i += 4;
	}
	free(data);
}

/*
** Определение расшифорвки или шифровки.
*/
void	working_data(t_base64 *base, char *data, size_t size)
{
	if (base->flags[base_d])
		decoding(base, data, size);
	else
		encoding(base, data, size);
}
