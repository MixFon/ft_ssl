#include "../include/md5.h"

void	stage_one(t_uint *vars, int i)
{
	vars[F] = (vars[B] & vars[C]) | ((~vars[B]) & vars[D]);
	vars[G] = i;
}

void	stage_two(t_uint *vars, int i)
{
	vars[F] = (vars[D] & vars[B]) | ((~vars[D]) & vars[C]);
	vars[G] = (5 * i + 1) % 16;
}

void	stage_three(t_uint *vars, int i)
{
	vars[F] = vars[B] ^ vars[C] ^ vars[D];
	vars[G] = (3 * i + 5) % 16;
}

void	stage_four(t_uint *vars, int i)
{
	vars[F] = vars[C] ^ (vars[B] | (~vars[D]));
	vars[G] = (7 * i) % 16;
}

void	init_start_vars(t_md *md)
{
	md->vars[A] = md->hash[A];
	md->vars[B] = md->hash[B];
	md->vars[C] = md->hash[C];
	md->vars[D] = md->hash[D];
	md->vars[F] = 0;
	md->vars[G] = 0;
}
