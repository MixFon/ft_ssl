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

int	main(int ac, const char *av[])
{
	t_ssl	ssl;

	init_ssl(&ssl);
	check_name_algorithm(&ssl, ac, av);
	fill_flags(&ssl, ac, av);
	run(&ssl);
	deinit_ssl(&ssl);
	return (0);
}
