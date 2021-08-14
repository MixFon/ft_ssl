#include "../include/des.h"

/*
** Функция начальной перестановки IP.
*/
uint64_t	function_ip(uint64_t block)
{
	uint64_t	result;
	size_t		i;
	uint8_t		*ip;

	i = -1;
	result = 0;
	ip = get_ip();
	while (++i < 64)
	{
		result = result << 1;
		result = result | ((block >> (64 - ip[i])) & 0x1);
	}
	return (result);
}

/*
** Функция расширения 32-х битового вестора до 48 бит.
** Дублирует некоторые биты.
*/
uint64_t	function_e(uint32_t right)
{
	uint64_t	rezult;
	int			i;
	uint8_t		*e;

	e = get_e();
	rezult = 0;
	i = -1;
	while (++i < 48)
	{
		rezult = rezult << 1;
		rezult = rezult | ((right >> (32 - e[i])) & 0x1);
	}
	return (rezult);
}

/*
** Возвращает число из таблицы g_s.
** Первый и последний биты шестибитового числа определяют номер строки в g_s
** Четыре бита между ними определяют номер столбца.
*/
uint8_t	get_number_s(uint64_t val, int i)
{
	int	row;
	int	col;

	row = ((val & 0x20) >> 4) | (val & 0x1);
	col = (val >> 0x1) & 0xF;
	return (g_s[i][row][col]);
}

/*
** Функция перестановки P.
** Меняет биты местами согласно таблице p.
*/
uint32_t	function_p(uint32_t val)
{
	uint32_t	rezult;
	uint8_t		*p;
	int			i;

	i = -1;
	rezult = 0;
	p = get_p();
	while (++i < 32)
	{
		rezult = rezult << 1;
		rezult = rezult | ((val >> (32 - p[i])) & 0x1);
	}
	return (rezult);
}

/*
** Основная функция шифрования, функция Фейстеля
** Шестибитные блоки должны идти слева направа 48 битного числа.
** Поэтому используется (block48 >> (6 * (7 - i))) & 0x3F)
*/
uint32_t	function_f(uint32_t right, uint64_t key)
{
	uint64_t	block48;
	int			i;
	uint32_t	rezult;
	uint8_t		bits4_s;

	i = -1;
	rezult = 0;
	block48 = function_e(right);
	block48 = block48 ^ key;
	while (++i < 8)
	{
		bits4_s = get_number_s(((block48 >> (6 * (7 - i))) & 0x3F), i);
		rezult = rezult << 4;
		rezult = rezult | bits4_s;
	}
	rezult = function_p(rezult);
	return (rezult);
}
