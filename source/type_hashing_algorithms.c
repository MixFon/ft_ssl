#include "../include/ft_ssl.h"

/*
** Выполнение программ по хешированию
*/
void	type_hashing_algorithms(int ac, const char **av)
{
	t_ssl	ssl;

	init_ssl(&ssl);
	check_name_algorithm(&ssl, ac, av);
	fill_flags(&ssl, ac, av);
	run(&ssl);
	deinit_ssl(&ssl);
}
