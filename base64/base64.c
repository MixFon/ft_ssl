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
		free(*data);
		*data = new_data;
	}
}

void	print_octest(void *data, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		ft_printf("%2.2x", *(((char *)data) + i));
		i++;
	}
	ft_printf("\n");
}

/*
** Заполнение шифра.
*/
void	fill_ciphertext(t_base64 *base, uint8_t *numbers)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		base->chiphertext[base->size_chiphertext + i] = base->symbols[numbers[i]];
		//ft_printf("number[%d] = {%d}\n", i, numbers[i]);
	}
	base->size_chiphertext += 4;
}

/*
** Заполнение шифратекста.
*/
void	fill_plaintext(t_base64 *base, char *numbers)
{
	int	i;

	i = -1;
	while (++i < 3)
		base->chiphertext[base->size_chiphertext + i] = numbers[i];
	base->size_chiphertext += 3;
}

/*
** Кодирование информации.
*/
void	encoding(t_base64 *base, char *data, size_t size)
{
	size_t	i;
	char	*word;
	uint8_t	numbers[4];
	int		equal;

	i = 0;
	equal = 0;
	if (size % 3 != 0)
		equal = 3 - (size % 3);
	base->chiphertext = ft_strnew(size + size / 3 + 3);
	resize_data(&data, &size);
	while (i < size)
	{
		word = (data + i);
		//ft_printf("word[0] = {%x}\n", *word);
		numbers[0] = (word[0] & 0xff) >> 2;
		//ft_printf("word[0] >> 2 = {%x}\n", numbers[0]);
		numbers[1] = ((word[0] & 0x3) << 4) | ((word[1] & 0xff) >> 4);
		numbers[2] = ((word[1] & 0xf) << 2) | ((word[2] & 0xff) >> 6);
		numbers[3] = word[2] & 0x3f;
		//print_bits(numbers, 4);
		//ft_printf("0 = {%d}, 1 = [%d] 2 = {%d} 3 = {%d}\n", numbers[0], numbers[1],numbers[2],numbers[3] );
		fill_ciphertext(base, numbers);
		i += 3;
	}
	while (equal > 0)
		base->chiphertext[size + size / 3 - equal--] = '=';
	free(data);
}

/*
** Возвращает индекс в массиве символов base64.
*/
int	get_index(t_base64 *base, char c)
{
	int	i;

	i = -1;
	while (++i < 64)
	{
		if (base->symbols[i] == c)
			return (i);
	}
	return (UNKNOWN_SYMBOL);
}

/*
** Расшифровка данных.
*/
void	decoding(t_base64 *base, char *data, size_t size)
{
	size_t	i;
	char	*word;
	char	indexes[3];

	i = 0;
	base->chiphertext = ft_strnew(size);
	while (i < size)
	{
		word = data + i;
		indexes[0] = (get_index(base, word[0]) << 2)
			| ((get_index(base, word[1]) & 0x30) >> 4);
		indexes[1] = (get_index(base, word[1]) << 4)
			| ((get_index(base, word[2]) & 0x3c) >> 2);
		indexes[2] = (get_index(base, word[2]) << 6) | get_index(base, word[3]);
		fill_plaintext(base, indexes);
		i += 4;
	}
	free(data);
}

/*
** Определение расшифорвки или шифровки.
*/
void	working_data(t_base64 *base, char *data, size_t size)
{
	if (base->flags[base_d])
		decoding(base, data, size);
	else
		encoding(base, data, size);
}

void	output_in_file(t_base64 *base)
{
	int		fd;
	ssize_t	len;

	fd = open(base->output_file, O_WRONLY | O_CREAT | O_TRUNC,
			  S_IWRITE | S_IREAD);
	if (fd < 0)
	{
		ft_putstr_fd("Error open file: ", 2);
		sys_err(base->output_file);
	}
	len = write(fd, base->chiphertext, base->size_chiphertext);
	if (len != base->size_chiphertext)
		sys_err("Error write file\n");
	//write(fd, "\n", 1);
	close(fd);
}

void	output_in_console(t_base64 *base)
{
	write(1, base->chiphertext, base->size_chiphertext);
	//write(0, "\n", 1);
}

/*
** Вывод шифротекст или шифр в зависимости от флага.
** В консоль или в файл.
*/
void	output_chiphertext(t_base64 *base)
{
	if (base->flags[base_o])
		output_in_file(base);
	else
		output_in_console(base);
}

/*
** Получение информации в зависимости от флагов.
*/
void	working_base64(t_base64 *base)
{
	char	*data;
	size_t	read_octets;

	read_octets = 0;
	if (base->flags[base_i])
		working_file_base64(base, &data, &read_octets);
	else
		working_stdin_base64(base, &data, &read_octets);
	working_data(base, data, read_octets);
	output_chiphertext(base);
}

void	deinit_base64(t_base64 *base)
{
	ft_strdel(&base->input_file);
	ft_strdel(&base->output_file);
	ft_strdel(&base->symbols);
	ft_strdel(&base->chiphertext);
}

void	init_base64(t_base64 *base)
{
	ft_memset(base, 0, sizeof(t_base64));
	base->symbols = ft_multi_strdup(3, "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			"abcdefghijklmnopqrstuvwxyz", "0123456789+/");
}

void	type_base64(int ac, const char **av)
{
	t_base64	base;

	init_base64(&base);
	read_flags(&base, ac, av);
	working_base64(&base);
	deinit_base64(&base);
}
