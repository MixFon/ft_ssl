#include "../include/ft_ssl.h"
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
