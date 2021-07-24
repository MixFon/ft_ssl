
#include "../include/ft_ssl.h"


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
