#include "../include/des.h"

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
int	is_hex_string(const char *str)
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
