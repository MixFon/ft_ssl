
#include "../include/ft_ssl.h"


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
