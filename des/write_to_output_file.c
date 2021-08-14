#include "../include/des.h"

/*
** Запись в файл.
*/
void	write_to_output_file(t_des *des)
{
	int		fd;
	ssize_t	len;

	fd = open(des->output_file, O_WRONLY | O_CREAT | O_TRUNC,
			  S_IWRITE | S_IREAD);
	if (fd < 0)
	{
		ft_putstr_fd("Error open file: ", 2);
		sys_err(des->output_file);
	}
	len = write(fd, des->output_message, des->size_message);
	if (len != des->size_message)
		sys_err("Error write file\n");
	close(fd);
}

/*
** При поднятых флагах -a -e необходимо ВЫХОДНОЙ поток зашифровать по base64
** Это значит, что на выходе должна быть текс в виде base64 строки.
*/
void	encode_base64(t_des *des)
{
	t_base64	base;

	if (des->flags[des_a] && !des->flags[des_d])
	{
		init_base64(&base);
		encoding(&base, (char *)des->output_message, des->size_message);
		des->output_message = (uint8_t *)ft_strnew(base.size_chiphertext);
		ft_memcpy(des->output_message, base.chiphertext, base.size_chiphertext);
		des->size_message = base.size_chiphertext;
		ft_strdel(((char **)(&base.chiphertext)));
		deinit_base64(&base);
	}
}

/*
** В режиме шифрования, в начало сообщения добавляется Salted__8октетов_соли
** Если не задан ключ.
*/
void	add_salt_to_output_message(t_des *des)
{
	uint8_t	*new;
	int		i;

	i = -1;
	if (!des->flags[des_k] && !des->flags[des_d])
	{
		new = (uint8_t *)ft_strnew(des->size_message + 20);
		ft_memcpy(new, "Salted__", 8);
		while (++i < 8)
			new[i + 8] = (des->salt >> (7 - i) * 8) & 0xff;
		ft_memcpy(new + 16, des->output_message, des->size_message);
		free(des->output_message);
		des->output_message = new;
		des->size_message += 16;
	}
}

void	write_output_message(t_des *des)
{
	add_salt_to_output_message(des);
	encode_base64(des);
	if (des->flags[des_o])
		write_to_output_file(des);
	else
		write(1, des->output_message, des->size_message);
}

t_mode	*get_modes(void)
{
	static t_mode	modes[] = {
		{mode_des_ecb, "des-ecb"},
		{mode_des_cbc, "des-cbc"},
		{mode_des_cbc, "des"},
		{mode_des3, "des3"},
		{mode_des_pcbc, "des-pcbc"},
		{mode_des_ofb, "des-ofb"},
		{mode_des_cfb, "des-cfb"},
		{mode_des_crt, "des-ctr"},
		{mode_simple, "simple"},
		{0, 0}
	};

	return (modes);
}
