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

void	print_error(const char *str)
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
		print_error(av[*i]);
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
		print_error(&av[*i][1]);
}

void	read_flags(t_base64 *base, int ac, const char **av)
{
	int	i;

	i = 1;
	while (++i < ac)
	{
		if (av[i][0] == '-')
			set_flag(base, ac, av, &i);
		else
			print_error(av[i]);
	}
	if (!(base->flags[base_d] ^ base->flags[base_e]))
		print_error("\0");
}

void	working_file_base64(t_base64 *base, char **data, size_t *read_octets)
{
	int		fd;

	fd = open(base->input_file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error open file: ", 2);
		sys_err(base->input_file);
	}
	else
	{
		*data = get_data_fd(fd, read_octets);
		close(fd);
	}
}

void	working_stdin_base64(t_base64 *base, char **data, size_t *read_octets)
{
	*data = get_data_fd(0, read_octets);
}

/*
** Выравнивание длины сообщения до кратности 3
*/
void	resize_data(char **data, size_t *size)
{
	char	*new_data;

	if (*size % 3 != 0)
	{
		new_data = ft_strnew(*size + (3 - *size % 3));
		ft_memcpy(new_data, *data, *size);
		*size = *size + (3 - *size % 3);
		ft_strdel(data);
		*data = new_data;
	}
}

void	print_octest(void *data, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size) {
		ft_printf("%2.2x", *(((char *)data) + i));
		i++;
	}
	ft_printf("\n");
}

void	fill_word(int *word, char *data)
{
	*word = 0;
	*word = *data;
	ft_printf("word = [%x]\n", *word);
	*word = *word << 8;
	ft_printf("word = [%x]\n", *word);
	*word = *word | *(data + 1);
	ft_printf("word = [%x]\n", *word);
	*word = *word << 8;
	ft_printf("word = [%x]\n", *word);
	*word = *word | *(data + 2);
	ft_printf("word = [%x]\n", *word);
}

void	fill_numbers(int word, int *numbers)
{
	numbers[0] = word >> 18;
	numbers[1] = (word >> 12) & 0x3f;
	numbers[2] = (word >> 6) & 0x3f;
	numbers[3] = word & 0x3f;
	ft_printf("0 = {%d}, 1 = {%d}, 2 = {%d}, 3 = [%d]\n", numbers[0], numbers[1], numbers[2], numbers[3]);
}

void	fill_ciphertext(t_base64 *base, int *numbers)
{
	int	i;

	i = -1;
	while (++i < 4)
		base->ciphertext[base->index + i] = base->symbols[numbers[i]];
	base->index += 4;
}

void	working_data(t_base64 *base, char *data, size_t size)
{
	size_t	i;
	int		word;
	int		numbers[4];
	int		equal;

	i = 0;
	equal = 0;
	if (size % 3 != 0)
		equal = 3 - (size % 3);
	ft_printf("size = [%d]\n", size);
	base->ciphertext = ft_strnew(size + size / 3 + 1);
	resize_data(&data, &size);
	ft_printf("size = [%d]\n", size);
	print_octest(data, size);
	while (i < size)
	{
		fill_word(&word, data + i);
		fill_numbers(word, numbers);
		fill_ciphertext(base, numbers);
		i += 3;
	}
	while (equal > 0)
	{
		ft_printf("equal = [%d]\n", equal);
		base->ciphertext[size + size / 3 - equal--] = '=';
	}
	ft_printf("%s\n", base->ciphertext);
}

void	working_base64(t_base64 *base)
{
	char	*data;
	size_t	read_octets;

	if (base->flags[base_i])
		working_file_base64(base, &data, &read_octets);
	else
		working_stdin_base64(base, &data, &read_octets);
	working_data(base, data, read_octets);
	//ft_printf("data = {%s} octets = [%d]\n", data, read_octets);
}

void	deinit_base64(t_base64 *base)
{
	ft_strdel(&base->input_file);
	ft_strdel(&base->output_file);
	ft_strdel(&base->symbols);
	ft_strdel(&base->ciphertext);
}

void	init_base64(t_base64 *base)
{
	ft_memset(base, 0, sizeof(t_base64));
	base->symbols = ft_multi_strdup(3, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			"abcdefghijklmnopqrstuvwxyz", "0123456789+/");
	ft_printf(base->symbols);
}

void	type_base64(int ac, const char **av)
{
	t_base64	base;

	init_base64(&base);
	read_flags(&base, ac, av);
	working_base64(&base);
	deinit_base64(&base);
}
