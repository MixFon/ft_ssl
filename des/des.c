#include "des.h"
#include "hmac.h"

void	init_des(t_des *des)
{
	ft_memset(des, 0, sizeof(t_des));
}

void	print_usage_des(void)
{
	ft_putstr_fd("Usage: ./ft_ssl des [-aed] [-i in_file] [-o out_file]\n", 2);
	ft_putstr_fd("\t[-k key] [-p password] [-s salt] [-v vector]\n", 2);
	ft_putstr_fd("-a, 			Perform base64 encoding/decoding\n", 2);
	ft_putstr_fd("-d, --decode	 decodes mode\n", 2);
	ft_putstr_fd("-e, --ecode	 ecodes mode\n", 2);
	ft_putstr_fd("-i, --input	 input file\n", 2);
	ft_putstr_fd("-o, --output	 output file\n", 2);
	ft_putstr_fd("-k, --key	key	 key specified as a hexadecimal string\n", 2);
	ft_putstr_fd("-p, --password password in ascii\n", 2);
	ft_putstr_fd("-s, --salt	 salt specified as a hexadecimal string\n", 2);
	ft_putstr_fd("-v, --vector	 specified as a hexadecimal string\n", 2);
	exit(-1);
}

/*
** Проверка состоит ли строка из шестнадцатиричных символов.
*/
int		is_hex_string(const char *str)
{
	while (*str != '\0')
	{
		if (ft_strchr(HEX_SYMBOLS, *str) == NULL)
			return (0);
		str++;
	}
	return (1);
}

/*
** Проверка на существование следующего аргумента.
*/
void	check_count_arguments(int i, int ac, const char **av)
{
	if (i >= ac)
	{
		ft_putstr_fd("des: option requires an argument -- ", 2);
		ft_putstr_fd(&av[i - 1][1], 2);
		ft_putstr_fd("\n", 2);
		print_usage_des();
	}
}

void	print_error_hex_string(const char *str)
{
	ft_putstr_fd("non-hex digit\n", 2);
	ft_putstr_fd("invalid hex ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(" value\n", 2);
	exit(-1);
}

int	get_hex_char_source(const char c)
{
	if (c == 'a' || c == 'A')
		return (0xa);
	if (c == 'b' || c == 'B')
		return (0xb);
	if (c == 'c' || c == 'C')
		return (0xc);
	if (c == 'd' || c == 'D')
		return (0xd);
	if (c == 'e' || c == 'E')
		return (0xe);
	if (c == 'f' || c == 'F')
		return (0xf);
	exit(-1);
}

int	get_hex_char(const char c)
{
	if (c == '1')
		return (0x1);
	if (c == '2')
		return (0x2);
	if (c == '3')
		return (0x3);
	if (c == '4')
		return (0x4);
	if (c == '5')
		return (0x5);
	if (c == '6')
		return (0x6);
	if (c == '7')
		return (0x7);
	if (c == '8')
		return (0x8);
	if (c == '9')
		return (0x9);
	if (c == '0')
		return (0x0);
	return (get_hex_char_source(c));
}

uint64_t	convert_string_to_hex_number(const char *str)
{
	int			i;
	uint64_t	number;

	i = -1;
	number = 0;
	while (++i < 16)
	{
		number = number << 4;
		if (*str != '\0')
			number = number | get_hex_char(*str++);
	}
	return (number);
}

/*
** Заполнение 64 битного числа на основе строки представленной hex
*/
void	fill_uint64_t_des(uint64_t *number, int ac, const char **av, int *i)
{
	(*i)++;
	check_count_arguments(*i, ac, av);
	if (!is_hex_string(av[*i]))
		print_error_hex_string(av[*i]);
	*number = convert_string_to_hex_number(av[*i]);
	ft_printf("{%10.10llx}\n", *number);
}

/*
** Запоминате строки. Имя файла или пароля.
*/
void	fill_string_des(char **str, int ac, const char **av, int *i)
{
	(*i)++;
	check_count_arguments(*i, ac, av);
	*str = ft_strdup(av[*i]);
}

void	print_error_des(const char *str)
{
	ft_putstr_fd("des: invalid option -- ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	print_usage_des();
}

void	set_fags_with_string(t_des *des, int ac, const char **av, int *i)
{
	if (av[*i][1] == 'k')
	{
		des->flags[des_k] = 1;
		fill_uint64_t_des(&des->key, ac, av, i);
	}
	else if (av[*i][1] == 'p')
	{
		des->flags[des_p] = 1;
		fill_string_des(&des->password, ac, av, i);
	}
	else if (av[*i][1] == 's')
	{
		des->flags[des_s] = 1;
		fill_uint64_t_des(&des->salt, ac, av, i);
	}
	else if (av[*i][1] == 'v')
	{
		des->flags[des_v] = 1;
		fill_uint64_t_des(&des->init_vector, ac, av, i);
	}
	else
		print_error_des(&av[*i][1]);
}


void	set_flag_des(t_des *des, int ac, const char **av, int *i)
{
	if (ft_strlen(av[*i]) != 2)
		print_error_des(av[*i]);
	if (av[*i][1] == 'a')
		des->flags[des_a] = 1;
	else if (av[*i][1] == 'e')
		des->flags[des_e] = 1;
	else if (av[*i][1] == 'd')
		des->flags[des_d] = 1;
	else if (av[*i][1] == 'i')
	{
		des->flags[des_i] = 1;
		fill_string_des(&des->input_file, ac, av, i);
	}
	else if (av[*i][1] == 'o')
	{
		des->flags[des_o] = 1;
		fill_string_des(&des->output_file, ac, av, i);
	}
	else
		set_fags_with_string(des, ac, av, i);
}

void	read_flags_des(t_des *des, int ac, const char **av)
{
	int	i;

	i = 1;
	while (++i < ac)
	{
		if (av[i][0] == '-')
			set_flag_des(des, ac, av, &i);
		else
			print_error_des(av[i]);
	}
	if (des->flags[des_d] && des->flags[des_e])
		print_error_des("\0");
}

void	deinit_des(t_des *des)
{
	ft_strdel(&des->input_file);
	ft_strdel(&des->output_file);
	ft_strdel(&des->password);
}

/*
** Заполняет соль на основе буфера.
*/
void	fill_salt(t_des *des, const char *buff)
{
	int	i;

	i = -1;
	while (++i < 16)
	{
		des->salt = des->salt << 4;
		des->salt = des->salt | (buff[i] & 0xf);
	}
}

/*
** Функция генерирует соль.
** Открывает файл с белым шумом и читает из него 8 окткетов.
*/
void	generate_salt(t_des *des)
{
	int		fd;
	char	buff[8];
	ssize_t	len;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		sys_err("Error open /dev/rendom");
	len = read(fd, buff, 8);
	close(fd);
	fill_salt(des, buff);
	des->flags[des_s] = 1;
}

/*
** Чтение пародя из входного патока.
** Запись пароля.
*/
void	read_password(t_des *des)
{
	char	*one;
	char	*two;

	one = ft_strdup(getpass("enter des-ecb encryption password:"));
	if (one == NULL)
		sys_err("Error malloc");
	two = ft_strdup(getpass("Verifying - enter des-ecb encryption password:"));
	if (two == NULL)
		sys_err("Error malloc");
	if (ft_strcmp(one, two))
		sys_err("Verify failure\nbad password read\n");
	ft_strdel(&two);
	des->password = one;
}

/*
** Функция генерирует ключ по алгоритму PBKDS
*/
void	generate_key(t_des *des)
{
	if (!des->flags[des_s])
		generate_salt(des);
	if (!des->password)
		read_password(des);
}

void	run_des(t_des *des)
{
	if (!des->flags[des_k])
		generate_key(des);
}

void	type_des(int ac, const char **av)
{
	t_des	des;

	t_uchar *temp = hmac_md5("key", "The quick brown fox jumps over the lazy dog");
	print_bits(temp, 16);
	free(temp);
	//ft_printf("{%s}\n", temp);
	exit(-1);
	init_des(&des);
	read_flags_des(&des, ac, av);
	run_des(&des);
	deinit_des(&des);
}
