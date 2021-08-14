#include "../include/des.h"

void	print_error_des(const char *str)
{
	ft_putstr_fd("des: invalid option -- ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	print_usage_des();
}

void	set_fags_with_string(t_des *des, int ac, const char **av, int *i)
{
	if (av[*i][1] == 'k')
	{
		des->flags[des_k] = 1;
		fill_uint64_t_des(&des->key, ac, av, i);
	}
	else if (av[*i][1] == 'p')
	{
		des->flags[des_p] = 1;
		fill_string_des(&des->password, ac, av, i);
	}
	else if (av[*i][1] == 's')
	{
		des->flags[des_s] = 1;
		fill_uint64_t_des(&des->salt, ac, av, i);
	}
	else if (av[*i][1] == 'v')
	{
		des->flags[des_v] = 1;
		fill_uint64_t_des(&des->init_vector, ac, av, i);
	}
	else if (av[*i][1] == 'w')
		des->flags[des_w] = 1;
	else
		print_error_des(&av[*i][1]);
}

void	set_flag_des(t_des *des, int ac, const char **av, int *i)
{
	if (ft_strlen(av[*i]) != 2)
		print_error_des(av[*i]);
	if (av[*i][1] == 'a')
		des->flags[des_a] = 1;
	else if (av[*i][1] == 'e')
		des->flags[des_e] = 1;
	else if (av[*i][1] == 'd')
		des->flags[des_d] = 1;
	else if (av[*i][1] == 'i')
	{
		des->flags[des_i] = 1;
		fill_string_des(&des->input_file, ac, av, i);
	}
	else if (av[*i][1] == 'o')
	{
		des->flags[des_o] = 1;
		fill_string_des(&des->output_file, ac, av, i);
	}
	else
		set_fags_with_string(des, ac, av, i);
}

/*
** Чтение флагов и запоминание их результатов.
*/
void	read_flags_des(t_des *des, int ac, const char **av)
{
	int	i;

	i = 1;
	while (++i < ac)
	{
		if (av[i][0] == '-')
			set_flag_des(des, ac, av, &i);
		else
			print_error_des(av[i]);
	}
	if (des->flags[des_d] && des->flags[des_e])
		print_error_des("\0");
	if (!des->flags[des_d] && !des->flags[des_e])
		des->flags[des_e] = 1;
}

void	deinit_des(t_des *des)
{
	ft_strdel(&des->input_file);
	ft_strdel(&des->output_file);
	ft_strdel(&des->password);
	ft_strdel((char **)(&des->message));
	ft_strdel((char **)(&des->output_message));
}
