#include "../include/base64.h"

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
	close(fd);
}

void	output_in_console(t_base64 *base)
{
	write(1, base->chiphertext, base->size_chiphertext);
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
