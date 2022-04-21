#include "../include/base64.h"

#include "base64.h"

void	print_usage_base64(void)
{
	ft_putstr_fd("Usage: ./ft_ssl base64 [-ed] [-i in_file] [-o out_file]\n", 2);
	ft_putstr_fd("-d, --decode	decodes mode\n", 2);
	ft_putstr_fd("-e, --ecode	ecodes mode\n", 2);
	ft_putstr_fd("-i, --input	input file\n", 2);
	ft_putstr_fd("-o, --output	output file\n", 2);
	exit(-1);
}

void	print_error_base64(const char *str)
{
	ft_putstr_fd("base64: invalid option -- ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	print_usage_base64();
}

void	fill_name_file(char **name_file, int ac, const char **av, int *i)
{
	(*i)++;
	if (*i >= ac)
	{
		ft_putstr_fd("base64: option requires an argument -- ", 2);
		ft_putstr_fd(&av[*i - 1][1], 2);
		ft_putstr_fd("\n", 2);
		print_usage_base64();
	}
	*name_file = ft_strdup(av[*i]);
}

void	set_flag(t_base64 *base, int ac, const char **av, int *i)
{
	if (ft_strlen(av[*i]) != 2)
		print_error_base64(av[*i]);
	if (av[*i][1] == 'e')
		base->flags[base_e] = 1;
	else if (av[*i][1] == 'd')
		base->flags[base_d] = 1;
	else if (av[*i][1] == 'i')
	{
		base->flags[base_i] = 1;
		fill_name_file(&base->input_file, ac, av, i);
	}
	else if (av[*i][1] == 'o')
	{
		base->flags[base_o] = 1;
		fill_name_file(&base->output_file, ac, av, i);
	}
	else
		print_error_base64(&av[*i][1]);
}

/*
** Обработка аргументов командной строки. Чтение флагов
*/
void	read_flags(t_base64 *base, int ac, const char **av)
{
	int	i;

	i = 1;
	while (++i < ac)
	{
		if (av[i][0] == '-')
			set_flag(base, ac, av, &i);
		else
			print_error_base64(av[i]);
	}
	if (base->flags[base_d] && base->flags[base_e])
		print_error_base64("\0");
}
