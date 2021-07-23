#include "ft_printf.h"
#include "ft_ssl.h"

/*
** Обнуление структуры.
*/
void	init_ssl(t_ssl *ssl)
{
	ft_memset(ssl, 0, sizeof(t_ssl));
}

void	help(void)
{
	ft_putstr_fd("Commands:\n", 2);
	ft_putstr_fd("md5\nsha256\n", 2);
	ft_putstr_fd("\nFlags:\n", 2);
	ft_putstr_fd("-p\n-q\n-r\n-s\n", 2);
	exit(-1);
}

/*
** Вывод сообщение об использовании.
*/
void	print_usage(t_ssl *ssl, const char c, const char *message)
{
	ft_putstr_fd(ssl->alg->name, 2);
	ft_putstr_fd(message, 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\nusage: ", 2);
	ft_putstr_fd(ssl->alg->name, 2);
	ft_putstr_fd(" [-pqr] [-s string] [files ...]\n\n", 2);
	help();
	exit(-1);
}

/*
** Проверяет допустимые флаги. Флаги могут быть pqrs
*/
void	check_valid_flags(t_ssl *ssl, const char *arg)
{
	while (*arg != '\0')
	{
		if (*arg != 'p' && *arg != 'q' && *arg != 'r' && *arg != 's')
			print_usage(ssl, *arg, ": illegal option -- ");
		arg++;
	}
}

void	print_array_string(char **arr, size_t len)
{
	size_t	i;

	i = 0;
	ft_printf("array:\n");
	while (i < len)
	{
		ft_printf("[%s]\n", arr[i]);
		i++;
	}
}

/*
** Добавление новой строки в указынный массив строк.
** Используется для добавление в strings и file_names
*/
void	add_string_to_array(char ***array, size_t *count,
		const char *new_string)
{
	size_t	i;
	char	**new_arr;

	i = 0;
	new_arr = (char **)malloc(sizeof(char *) * (*count + 1));
	while (i < *count)
	{
		new_arr[i] = ft_strdup((*array)[i]);
		ft_strdel(&((*array)[i]));
		i++;
	}
	new_arr[i] = ft_strdup(new_string);
	free(*array);
	*array = new_arr;
	(*count)++;
}

/*
** Добавление новой строки в массив строк.
*/
void	add_string(t_ssl *ssl, int ac, const char **av)
{
	ssl->i++;
	if (ssl->i == ac)
		print_usage(ssl, 's', ": option requires an argument -- ");
	add_string_to_array(&ssl->strings, &ssl->count_strings, av[ssl->i]);
}

int	is_check(t_ssl *ssl, const char *arg)
{
	if (ft_strlen(arg) == 1)
	{
		ssl->k = 2;
		return (1);
	}
	return (0);
}

/*
** Происходит прверка флага. Если длина равно 1 (т.е. пришла только '-').
** Значит в аргументах идут имена файлов.
*/
void	read_flag(t_ssl *ssl, int ac, const char **av)
{
	const char	*arg;

	arg = av[ssl->i];
	if (is_check(ssl, arg))
		return ;
	arg++;
	check_valid_flags(ssl, arg);
	while (*arg != '\0')
	{
		if (*arg == 'p')
			ssl->flags[P] = 1;
		else if (*arg == 'q')
			ssl->flags[Q] = 1;
		else if (*arg == 'r')
			ssl->flags[R] = 1;
		else if (*arg == 's')
		{
			ssl->flags[S] = 1;
			add_string(ssl, ac, av);
			return ;
		}
		arg++;
	}
}

/*
** Добавляет новое имя файла в массив имен файлов.
*/
void	add_file_name(t_ssl *ssl, const char **av, int i)
{
	add_string_to_array(&ssl->file_names, &ssl->count_file_names, av[i]);
}

/*
** Заполнение флагов программы.
*/
void	fill_flags(t_ssl *ssl, int ac, const char **av)
{
	ssl->i = 1;
	ssl->k = 0;
	while (++ssl->i < ac)
	{
		if (av[ssl->i][0] == '-' && ssl->k < 2)
			read_flag(ssl, ac, av);
		else
			ssl->k = 2;
		if (ssl->k == 2)
			add_file_name(ssl, av, ssl->i);
	}
}

/*
** Проверка имени алгоритма.
*/
void	check_name_algorithm(t_ssl *ssl, int ac, const char **av)
{
	t_alg	*algs;

	algs = get_algorithms();
	if (ac < 2)
		sys_err("usage: ft_ssl command [command opts] [command args]\n");
	while (algs->name != 0)
	{
		if (!ft_strcmp(av[1], algs->name))
		{
			ssl->alg = algs;
			return ;
		}
		algs++;
	}
	ft_putstr_fd("ft_ssl: Error: \'", 2);
	ft_putstr_fd(av[1], 2);
	ft_putstr_fd("\' is an invalid command.\n", 2);
	help();
	exit(-1);
}

/*
** Освобождение памяти, выделенные под массивы строк.
*/
void	delete_array_string(char ***array, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		ft_strdel(&(*array)[i]);
		i++;
	}
	free(*array);
}

void	deinit_ssl(t_ssl *ssl)
{
	delete_array_string(&ssl->strings, ssl->count_strings);
	delete_array_string(&ssl->file_names, ssl->count_file_names);
}

/*
** Возвращает содержимое файла в виде строки.
** Память выделяется динамически.
*/
char	*get_data_fd(int fd, size_t *len_message_oct)
{
	char	buff[BUFF + 1];
	ssize_t	oct_read;
	char	*data;
	char	*temp;

	data = ft_strnew(0);
	*len_message_oct = 0;
	while (21)
	{
		oct_read = read(fd, buff, BUFF);
		if (oct_read <= 0)
			break ;
		buff[oct_read] = '\0';
		temp = ft_strnew(*len_message_oct + oct_read);
		ft_memcpy(temp, data, *len_message_oct);
		ft_memcpy(temp + *len_message_oct, buff, oct_read);
		ft_strdel(&data);
		data = temp;
		ft_memset(buff, 0, BUFF);
		*len_message_oct += oct_read;
	}
	return (data);
}

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

/*
** Взятие информации из входного потока.
*/
void	working_stdin(t_ssl *ssl)
{
	char	*data;
	t_uchar	*hash;

	data = get_data_fd(0, &ssl->len_message_oct);
	hash = get_hash((const t_uchar *)data, ssl);
	if (ssl->flags[P])
		ft_putstr(data);
	print_hash(ssl, hash);
	free(hash);
	ft_strdel(&data);
}

void	print_hash_strings_files(t_ssl *ssl, const t_uchar *data,
		const char *info)
{
	t_uchar	*hash;
	int		i;

	i = -1;
	hash = get_hash(data, ssl);
	if (ssl->flags[Q])
		print_hash(ssl, hash);
	else if (ssl->flags[R])
	{
		while (++i < ssl->alg->size)
			ft_printf("%02x", hash[i]);
		ft_printf(" %s\n", info);
	}
	else
	{
		ft_printf("%s (%s) = ", ssl->alg->name_case, info);
		print_hash(ssl, hash);
	}
	free(hash);
}

void	working_strings(t_ssl *ssl)
{
	size_t	i;
	char	*info;

	i = 0;
	while (i < ssl->count_strings)
	{
		info = ft_multi_strdup(3, "\"", ssl->strings[i], "\"");
		ssl->len_message_oct = ft_strlen(ssl->strings[i]);
		print_hash_strings_files(ssl, (const t_uchar *)ssl->strings[i], info);
		ft_strdel(&info);
		i++;
	}
}

/*
** Определение, является ли файл папкой.
** В условиял сказано, что можно использовать функции,
** если их использование обосновано.
** MD5 не вычисляет хеш папок.
*/
int	is_dir(const char *name_file)
{
	struct stat	buf;

	lstat(name_file, &buf);
	return (S_ISDIR(buf.st_mode));
}

void	open_file(t_ssl *ssl, size_t i)
{
	int		fd;
	char	*data;

	fd = open(ssl->file_names[i], O_RDONLY);
	if (fd < 0)
		ft_printf("%s: %s: Error open file.\n",
			ssl->alg->name, ssl->file_names[i]);
	else
	{
		data = get_data_fd(fd, &ssl->len_message_oct);
		print_hash_strings_files(ssl, (const t_uchar *)data,
			ssl->file_names[i]);
		ft_strdel(&data);
		close(fd);
	}
}

/*
** Вычисление хеш файлов. Папки не обрабатываются.
*/
void	working_files(t_ssl *ssl)
{
	size_t	i;

	i = 0;
	while (i < ssl->count_file_names)
	{
		if (is_dir(ssl->file_names[i]))
			ft_printf("%s: %s: Is a directory.\n",
				ssl->alg->name, ssl->file_names[i]);
		else
			open_file(ssl, i);
		i++;
	}
}

void	run(t_ssl *ssl)
{
	if (ssl->flags[P] || (!ssl->flags[S] && ssl->count_file_names == 0))
		working_stdin(ssl);
	if (ssl->flags[S])
		working_strings(ssl);
	if (ssl->count_file_names != 0)
		working_files(ssl);
}

/*
** Возвращает массив структур с указателями на функцию.
** Для добавления новой функции достаточно будет добавить новую структуру.
*/
t_alg	*get_algorithms(void)
{
	static t_alg	algorithms[] = {
		{alg_md5, "md5", "MD5", 16},
		{alg_sha256, "sha256", "SHA256", 32},
		{0, 0, 0, 0},
	};

	return (algorithms);
}

int	main(int ac, const char *av[])
{
	t_ssl	ssl;

	int i = 0;
	while (++i < 10)
	{
		char *temp = ft_strnew(10);
		temp = NULL;
		ft_printf("Hello");
	}
	init_ssl(&ssl);
	check_name_algorithm(&ssl, ac, av);
	fill_flags(&ssl, ac, av);
	run(&ssl);
	deinit_ssl(&ssl);
	return (0);
}
