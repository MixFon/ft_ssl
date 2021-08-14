#include "../include/des.h"

uint8_t	*get_ip_final(void)
{
	static uint8_t	ip_final[] = {
		40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
	};

	return (ip_final);
}

/*
** Возвращает массив сдвигов для циклического сдвина на 28.
*/
uint8_t	*count_shift(void)
{
	static uint8_t	shift[] = {
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

	return (shift);
}

/*
** Циклический сдвиг влево.
*/
void	shift_to_left_28(uint32_t *c, uint32_t *d, int round)
{
	uint8_t	*shift;
	uint8_t	mask;

	shift = count_shift();
	if (shift[round] == 1)
		mask = 0x1;
	else
		mask = 0x3;
	*c = ((*c << shift[round]) & 0xFFFFFFF)
		| ((*c >> (28 - shift[round])) & mask);
	*d = ((*d << shift[round]) & 0xFFFFFFF)
		| ((*d >> (28 - shift[round])) & mask);
}

/*
** Расширение ключа до 56 бит.
*/
void	working_pc1(t_des *des)
{
	uint8_t		*pc1;
	uint64_t	result;
	int			i;

	pc1 = get_pc1();
	i = -1;
	des->key56 = 0;
	result = 0;
	while (++i < 56)
	{
		des->key56 = des->key56 << 1;
		des->key56 |= (des->key >> (64 - pc1[i])) & 0x1;
	}
}

/*
** Перестановка со сжатием и формирование i-го ключа.
*/
void	working_pc2(t_des *des, int i)
{
	uint64_t	key56;
	uint8_t		*pc2;
	int			j;

	j = -1;
	key56 = des->c0;
	key56 = key56 << 28;
	key56 = key56 | des->d0;
	pc2 = get_pc2();
	des->keys[i] = 0;
	while (++j < 48)
	{
		des->keys[i] = des->keys[i] << 1;
		des->keys[i] = des->keys[i] | ((key56 >> (56 - pc2[j])) & 0x1);
	}
}
