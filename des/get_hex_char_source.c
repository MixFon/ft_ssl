#include "../include/des.h"

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
