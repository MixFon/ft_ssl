#include "../include/des.h"

/*
** Определение режима работы алгоритма.
** Режимы работы:
** des-ecb - режим электронной книги.
** des-cbc - режим связных блоков.
** des3 - троекратный des
*/
void	determining_operating_mode(t_des *des, const char **av)
{
	t_mode	*modes;

	modes = get_modes();
	while ((*modes).mode_name != 0)
	{
		if (!ft_strcmp(av[1], (*modes).mode_name))
		{
			des->mode = modes;
			return ;
		}
		modes++;
	}
}

void	type_des(int ac, const char **av)
{
	t_des	des;

	init_des(&des);
	determining_operating_mode(&des, av);
	read_flags_des(&des, ac, av);
	run_des(&des);
	write_output_message(&des);
	deinit_des(&des);
}
