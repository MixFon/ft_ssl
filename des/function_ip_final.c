#include "../include/des.h"

uint64_t	function_ip_final(uint64_t block)
{
	uint64_t	rezult;
	uint8_t		*ip_final;
	int			i;

	i = -1;
	rezult = 0;
	ip_final = get_ip_final();
	while (++i < 64)
	{
		rezult = rezult << 1;
		rezult = rezult | ((block >> (64 - ip_final[i])) & 0x1);
	}
	return (rezult);
}

/*
** Записывает отктеты из блока в выходную строку.
*/
void	write_uint64_to_output_message(t_des *des, uint64_t block, size_t i)
{
	size_t	k;
	int		j;

	j = -1;
	k = i + 7;
	while (++j < 8)
		des->output_message[k--] = ((block >> (j * 8)) & 0xff);
}

/*
** Возвращет ключ в зависи от шифровки ли дешифровки.
** -e шифрует, -d дешифрует.
*/
uint64_t	get_kye(t_des *des, int i)
{
	uint64_t	key;

	if (des->flags[des_d])
		key = des->keys[15 - i];
	else
		key = des->keys[i];
	return (key);
}

uint64_t	function_des(t_des *des, uint64_t block64)
{
	uint32_t	vars[3];
	int			iter;
	uint64_t	key;

	iter = -1;
	block64 = function_ip(block64);
	vars[left] = (block64 >> 32);
	vars[right] = block64 & 0xFFFFFFFF;
	while (++iter < 16)
	{
		key = get_kye(des, iter);
		vars[temp] = vars[right];
		vars[right] = function_f(vars[right], key) ^ vars[left];
		vars[left] = vars[temp];
	}
	vars[temp] = vars[right];
	vars[right] = vars[left];
	vars[left] = vars[temp];
	block64 = 0;
	block64 = vars[left];
	block64 = block64 << 32;
	block64 = block64 | vars[right];
	block64 = function_ip_final(block64);
	return (block64);
}

/*
** При поднятых флагах -a -d необходимо входной поток расшифровать из base64
** Это значит, что на фход подается сообщение закодированное по base64.
*/
void	decode_base64(t_des *des)
{
	t_base64	base;
	size_t		i;
	int			size_equel;

	i = des->size_message;
	size_equel = 0;
	if (des->flags[des_a] && des->flags[des_d])
	{
		while (des->message[--i] == '=')
			size_equel++;
		init_base64(&base);
		decoding(&base, (char *)des->message, des->size_message);
		des->message = (uint8_t *)ft_strnew(base.size_chiphertext);
		ft_memcpy(des->message, base.chiphertext, base.size_chiphertext);
		des->size_message = base.size_chiphertext - size_equel;
		ft_strdel(((char **)(&base.chiphertext)));
		deinit_base64(&base);
	}
}
