#include "../include/base64.h"

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
