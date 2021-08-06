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
	ft_putstr_fd("-w, 			 display the salt and key\n", 2);
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
	else if (av[*i][1] == 'w')
		des->flags[des_w] = 1;
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

/*
** Чтение флагов и запоминание их результатов.
*/
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
	ft_strdel((char **)(&des->message));
	ft_strdel((char **)(&des->output_message));
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

t_uchar	*first_step(char *password, const uint64_t salt_num)
{
	t_uchar	*salt;
	int		i;
	t_uchar	*u;

	i = -1;
	salt = (t_uchar *)ft_strnew(16);
	ft_printf("salt_num {%8.8llx}\n", salt_num);
	while (++i < 8)
		salt[i] = 0xff & (salt_num >> (64 - (i + 1) * 8));
	salt[15] = 1;
	print_bits(salt, 16);
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

/*
** Переволит ключ в виде строки в число.
*/
void	fill_key(t_des *des, t_uchar *result)
{
	des->key = string_to_uinit64(result);
	des->flags[des_k] = 1;
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

/*
** Функция генерирует ключ по алгоритму PBKDS
*/
void	generate_key(t_des *des)
{
	if (!des->flags[des_s])
		generate_salt(des);
	if (!des->password)
		read_password(des);
	pbkdf(des);
	if (des->flags[des_w])
		ft_printf("salt=%8.8llX\nkey=%8.8llX\n", des->salt, des->key);
}

uint8_t	*get_pc1(void)
{
	static uint8_t	pc1[] = {
		57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4,
	};

	return (pc1);
}

uint8_t	*get_pc2(void)
{
	static uint8_t	pc2[] = {
		14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
		26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
		51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
	};

	return (pc2);
}

uint8_t	*get_ip(void)
{
	static uint8_t	ip[] = {
		58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
	};

	return (ip);
}

uint8_t	*get_e(void)
{
	static uint8_t	e[] = {
		32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
	};

	return (e);
}

uint8_t	*get_p(void)
{
	static uint8_t	p[] = {
		16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25,
	};

	return (p);
}

uint8_t	*get_ip_final(void)
{
	static uint8_t	ip_final[] = {
		58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
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
void shift_to_left_28(uint32_t *c, uint32_t *d, int round)
{
	uint8_t	*shift;

	shift = count_shift();
	*c = ((*c << shift[round]) & 0xFFFFFFF) | ((*c >> (28 - shift[round])) & shift[round]);
	*d = ((*d << shift[round]) & 0xFFFFFFF) | ((*d >> (28 - shift[round])) & shift[round]);
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
	ft_printf("des->c0 = {%8.8x}\ndes->d0 = {%8.8x}\n", des->c0, des->d0);
	ft_printf("key56 = [%16.16llx]\n", key56);
	while (++j < 48)
	{
		des->keys[i] = des->keys[i] << 1;
		des->keys[i] = des->keys[i] | ((key56 >> (56 - pc2[j])) & 0x1);
		//ft_printf("j = {%d} pc2 = {%d}\n", j, pc2[47 - j]);
	}
	//ft_printf("des->keys[%d] = [%llx]\n", i, des->keys[i]);
}

/*
** Генерация ключей (16 ключей по 48 бит)
*/
void	generate_kays(t_des *des)
{
	int	i;

	i = -1;
	working_pc1(des);
	ft_printf("key_pc1 = {%16.16llx}\n", des->key56);
	des->c0 = (des->key56 >> 28) & 0xFFFFFFF;
	des->d0 = des->key56 & 0xFFFFFFF;
	ft_printf("des->c0 = {%8.8x}\ndes->d0 = {%8.8x}\n", des->c0, des->d0);
	while (++i < 16)
	{
		shift_to_left_28(&des->c0, &des->d0, i);
		working_pc2(des, i);
		ft_printf("key48b = {%16.16llx}\n", des->keys[i]);
	}
}

/*
** Выравнивание сообщения до длины кратной 8 октетам.
*/
void	resize_message(t_des *des)
{
	uint8_t	*temp;

	if (des->size_message % 8 != 0)
	{
		temp = (uint8_t *)ft_strnew(des->size_message + (8 - (des->size_message % 8)));
		ft_memcpy(temp, des->message, des->size_message);
		des->size_message += (8 - (des->size_message % 8));
		free(des->message);
		des->message = temp;
	}
	des->output_message = (uint8_t *)ft_strnew(des->size_message + 2);
}

/*
** Получение сообщения для шифрования из файла.
*/
void	working_file_des(t_des *des)
{
	int		fd;

	fd = open(des->input_file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error open file: ", 2);
		sys_err(des->input_file);
	}
	else
	{
		des->message = (uint8_t *)get_data_fd(fd, &des->size_message);
		close(fd);
	}
}

/*
** Получение сообщения для шифрования из входного потока.
*/
void	working_input_des(t_des *des)
{
	des->message = (uint8_t *)get_data_fd(0, &des->size_message);
}

/*
** Получение сообщения для шифрования из файла или из входного потока.
*/
void	get_message(t_des *des)
{
	if (des->flags[des_i])
		working_file_des(des);
	else
		working_input_des(des);
}

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
		result = result | ((block >> (ip[63 - i] - 1)) & 0x1);
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
		rezult = rezult | ((right >> (e[47 - i] - 1)) & 0x1);
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
	int col;

	row = ((val & 0x20) >> 4) | (val & 0x1);
	col = (val >> 0x1) & 0xF;
	//ft_printf("i = {%d} row = [%d] col = {%d} g_s = {%d}\n",i, row, col, g_s[i][row][col]);
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
		rezult = rezult | ((val >> (p[31 - i] - 1)) & 0x1);
	}
	return (rezult);
}

/*
** Основная функция шифрования, функция Фейстеля
*/
uint32_t	function_f(uint32_t right, uint64_t key)
{
	uint64_t	ex_right;
	int			i;
	uint32_t	rezult;
	uint8_t		bits4_s;

	i = -1;
	rezult = 0;
	ex_right = function_e(right);
	//ft_printf("right = {%llx}\n", right);
	//ft_printf("ex_right1 = {%llx}\n", ex_right);
	ex_right = ex_right ^ key;
	//ft_printf("ex_right = {%llx}\n", ex_right);
	while (++i < 8)
	{
		bits4_s = get_number_s((ex_right >> (6 * i) & 0x3F), i);
		rezult = rezult << 4;
		rezult = rezult | bits4_s;
	}
	rezult = function_p(rezult);
	return (rezult);
}

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
		rezult = rezult | ((block >> (ip_final[63 - i] - 1)) & 0x1);
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
	print_bits(des->output_message, des->size_message);
}

/*
** Возвращет ключ в зависи от шифровки ли дешифровки.
** -e шифрует, -d дешифрует.
*/
uint64_t	get_kye(t_des *des, int i)
{
	uint64_t	key;

	if (des->flags[des_e])
		key = des->keys[i];
	else
		key = des->keys[15 - i];
	return (key);
}

void	function_des(t_des *des, size_t i)
{
	uint64_t	block;
	uint32_t	vars[3];
	int			iter;
	uint64_t	key;

	iter = -1;
	block = string_to_uinit64(des->message + i);
	ft_printf("block \t[%llx]\n", block);
	print_bits(des->message + i, 8);
	block = function_ip(block);
	ft_printf("block \t[%llx]\n", block);
	vars[left] = (block >> 32);
	vars[right] = block & 0xFFFFFFFF;
	ft_printf("vars[left] \t[%llx]\n", vars[left]);
	ft_printf("vars[right] \t[%llx]\n", vars[right]);
	//ft_printf("temp_ip [%llx]\n", block);
	while (++iter < 16)
	{
		key = get_kye(des, iter);
		vars[temp] = vars[right];
		vars[right] = vars[left] ^ function_f(vars[right], key);
		vars[left] = vars[temp];
	}
	block = 0;
	block = vars[left];
	block = block << 32;
	block = block | vars[right];
	block = function_ip_final(block);
	write_uint64_to_output_message(des, block, i);
	ft_printf("block21 = {%llx}\n", block);
	print_bits((uint8_t *)(&block), 8);
	print_bits(des->output_message + i, 8);
}

void	run_des(t_des *des)
{
	size_t	i;

	i = 0;
	if (!des->flags[des_k])
		generate_key(des);
	ft_printf("keyhex = {%llx}\n", des->key);
	//print_bits(&des->key, 8);
	generate_kays(des);
	get_message(des);
	resize_message(des);
	ft_printf("message = [%s] size_message = {%d}\n", des->message, des->size_message);
	while (i < des->size_message)
	{
		function_des(des, i);
		i += 8;
	}
}

/*
** Запись в файл.
*/
void	write_to_output_file(t_des *des)
{
	int		fd;
	ssize_t	len;

	fd = open(des->output_file, O_WRONLY | O_CREAT | O_TRUNC,
			  S_IWRITE | S_IREAD);
	if (fd < 0)
	{
		ft_putstr_fd("Error open file: ", 2);
		sys_err(des->output_file);
	}
	len = write(fd, des->output_message, des->size_message);
	if (len != des->size_message)
		sys_err("Error write file\n");
	close(fd);
}

void	write_output_message(t_des *des)
{
	if (des->flags[des_o])
		write_to_output_file(des);
	else
		ft_printf("%s\n", des->output_message);
		
}

void	type_des(int ac, const char **av)
{
	t_des	des;

//	//ft_printf("key = {%s}, str = [%s]", av[2], av[3]);
//	t_uchar *temp = hmac_md5(av[2], av[3]);
//	//ARPHSALJUABSGWIEUAEVBXEVKMJRFYHZIBYTASLPEZ
//	//t_uchar *temp = hmac_md5("ARPHSALJUABSGWIEUAEVBXEVKMJRFYHZIBYTASLPEZ", "JHH");
//	print_bits(temp, 16);
//	free(temp);
//	//ft_printf("{%s}\n", temp);
//	exit(-1);
	init_des(&des);
	read_flags_des(&des, ac, av);
	run_des(&des);
	write_output_message(&des);
	deinit_des(&des);
}
