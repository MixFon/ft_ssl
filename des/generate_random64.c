#include "../include/des.h"

/*
** Функция генерирует соль.
** Открывает файл с белым шумом и читает из него 8 окткетов.
*/
void	generate_random64(uint64_t *salt)
{
	int		fd;
	ssize_t	len;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		sys_err("Error open /dev/rendom");
	len = read(fd, (uint8_t *)salt, 8);
	close(fd);
}

/*
** Чтение пародя из входного патока.
** Запись пароля.
*/
void	read_password(t_des *des)
{
	char	*one;
	char	*two;

	ft_printf("enter %s encryption password:", des->mode->mode_name);
	one = ft_strdup(getpass(""));
	if (one == NULL)
		sys_err("Error malloc");
	if (*one == '\n')
		exit(-1);
	ft_printf("Verifying - enter %s encryption password:",
		des->mode->mode_name);
	two = ft_strdup(getpass(""));
	if (two == NULL)
		sys_err("Error malloc");
	if (ft_strcmp(one, two))
		sys_err("Verify failure\nbad password read\n");
	ft_strdel(&two);
	des->password = one;
}

t_uchar	*first_step(char *password, const uint64_t salt_num)
{
	t_uchar	*salt;
	int		i;
	t_uchar	*u;

	i = -1;
	salt = (t_uchar *)ft_strnew(16);
	while (++i < 8)
		salt[i] = 0xff & (salt_num >> (64 - (i + 1) * 8));
	salt[15] = 1;
	u = hmac_md5(password, salt);
	free(salt);
	return (u);
}

/*
** Оперция исключающее ИЛИ над двумя строками.
*/
void	xor(t_uchar *result, t_uchar *u)
{
	int	i;

	i = -1;
	while (++i < 16)
		result[i] = result[i] ^ u[i];
}

/*
** Переводит 8 отктетов строки в число uint64_t
*/
uint64_t	string_to_uinit64(uint8_t *message)
{
	uint64_t	result;
	size_t		i;

	i = -1;
	result = 0;
	while (++i < 8)
	{
		result = result << 8;
		result = result | message[i];
	}
	return (result);
}
