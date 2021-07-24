#include "../include/ft_ssl.h"

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
