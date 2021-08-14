#include "../include/base64.h"

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
		base->chiphertext[base->size_chiphertext + i]
			= base->symbols[numbers[i]];
	base->size_chiphertext += 4;
}
