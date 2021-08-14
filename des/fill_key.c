#include "../include/des.h"

/*
** Переволит ключ в виде строки 8 октктов в 64 битное число.
*/
void	fill_key(t_des *des, t_uchar *result)
{
	des->key = string_to_uinit64(result);
}

/*
** kdLen = 8 oct
** l = 0
** r = 8
*/
void	pbkdf(t_des *des)
{
	int		i;
	uint8_t	*u;
	uint8_t	*temp;
	uint8_t	result[16];

	i = 1;
	u = first_step(des->password, des->salt);
	ft_memset(result, 0, 16);
	while (++i < 10)
	{
		xor(result, u);
		temp = hmac_md5(des->password, u);
		free(u);
		u = temp;
	}
	free(u);
	fill_key(des, result);
}

void	generate_init_vector(t_des *des)
{
	if (!des->flags[des_v])
	{
		generate_random64(&des->init_vector);
		des->flags[des_v] = 1;
	}
	if (des->flags[des_w])
	{
		if (ft_strcmp(des->mode->mode_name, "des-ecb"))
			ft_printf("salt=%8.8llX\nkey=%8.8llX\niv =%8.8llX\n",
				des->salt, des->key, des->init_vector);
		else
			ft_printf("salt=%8.8llX\nkey=%8.8llX\n", des->salt, des->key);
	}
}

/*
** Считывает соль из входного сообщения.
** В начале сообщения должно идти сообщение Salted__8октетов_соли
*/
void	read_salt_from_input_massage(t_des *des)
{
	int		i;

	i = -1;
	if (ft_memcmp(des->message, "Salted__", 8))
		sys_err("bad magic number\n");
	if (des->size_message < 16)
		sys_err("error reading input file\n");
	des->salt = 0;
	while (++i < 8)
	{
		des->salt = des->salt << 8;
		des->salt = des->salt | (des->message[i + 8] & 0xff);
	}
}

/*
** Функция генерирует ключ по алгоритму PBKDS
*/
void	generate_key(t_des *des)
{
	if (!des->flags[des_s])
	{
		if (des->flags[des_d])
			read_salt_from_input_massage(des);
		else
			generate_random64(&des->salt);
		des->flags[des_s] = 1;
	}
	pbkdf(des);
}
