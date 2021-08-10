#include "../include/ft_ssl.h"

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

/*
** Возвращает массив структур с указатемями на функцию и их именами.
*/
t_type	*get_type_programm(void)
{
	static t_type	type_programm[] = {
		{type_hashing_algorithms, "md5"},
		{type_hashing_algorithms, "sha256"},
		{type_base64, "base64"},
		{type_des, "des"},
		{type_des, "des-ecb"},
		{0, 0}
	};
	
	return (type_programm);
}

int	main(int ac, const char *av[])
{
	t_type	*type;

	type = get_type_programm();
	if (ac < 2)
		sys_err("usage: ft_ssl command [command opts] [command args]\n");
	while (type->name != 0)
	{
		if (!ft_strcmp(av[1], type->name))
		{
			type->funtion_type(ac, av);
			return (0);
		}
		type++;
	}
	ft_putstr_fd("ft_ssl: Error: \'", 2);
	ft_putstr_fd(av[1], 2);
	ft_putstr_fd("\' is an invalid command.\n", 2);
	help();
	return (-1);
}
