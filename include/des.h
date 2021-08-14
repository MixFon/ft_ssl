#ifndef DES_H
# define DES_H

# include "libft.h"
# include "base64.h"
# include "ft_ssl.h"

# define HEX_SYMBOLS "1234567890abcdefABCDEF"

# define KDLEN 8

typedef struct s_des	t_des;

typedef struct s_mode
{
	void		(*operating_mode)(t_des *des);
	const char	*mode_name;
}				t_mode;

typedef struct s_des
{
	int			flags[10];
	char		*input_file;
	char		*output_file;
	char		*password;
	uint8_t		*message;
	uint8_t		*output_message;
	size_t		size_message;
	uint64_t	key;
	uint64_t	key56;
	uint64_t	keys[16];
	uint64_t	salt;
	uint64_t	init_vector;
	uint32_t	c0;
	uint32_t	d0;
	t_mode		*mode;
}				t_des;

enum e_des
{
	des_a,
	des_d,
	des_e,
	des_i,
	des_k,
	des_o,
	des_p,
	des_s,
	des_v,
	des_w
};

enum e_side
{
	left,
	right,
	temp
};

static const uint8_t	g_s[8][4][16] = {
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
	{
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};

void		type_des(int ac, const char **av);
/*
** File init_des.c
*/
void		init_des(t_des *des);
void		print_usage_des(void);
int			is_hex_string(const char *str);
void		check_count_arguments(int i, int ac, const char **av);
void		print_error_hex_string(const char *str);
/*
** File get_hex_char_source.c
*/
int			get_hex_char_source(const char c);
int			get_hex_char(const char c);
uint64_t	convert_string_to_hex_number(const char *str);
void		fill_uint64_t_des(uint64_t *number, int ac,
				const char **av, int *i);
void		fill_string_des(char **str, int ac, const char **av, int *i);
/*
** File print_error_des.c
*/
void		print_error_des(const char *str);
void		set_fags_with_string(t_des *des, int ac, const char **av, int *i);
void		set_flag_des(t_des *des, int ac, const char **av, int *i);
void		read_flags_des(t_des *des, int ac, const char **av);
void		deinit_des(t_des *des);
/*
** File generate_random64.c
*/
void		generate_random64(uint64_t *salt);
void		read_password(t_des *des);
t_uchar		*first_step(char *password, const uint64_t salt_num);
void		xor(t_uchar *result, t_uchar *u);
uint64_t	string_to_uinit64(uint8_t *message);
/*
** File fill_key.c
*/
void		fill_key(t_des *des, t_uchar *result);
void		pbkdf(t_des *des);
void		generate_init_vector(t_des *des);
void		read_salt_from_input_massage(t_des *des);
void		generate_key(t_des *des);
/*
** File get_pc1.c
*/
uint8_t		*get_pc1(void);
uint8_t		*get_pc2(void);
uint8_t		*get_ip(void);
uint8_t		*get_e(void);
uint8_t		*get_p(void);
/*
** File get_ip_final.c
*/
uint8_t		*get_ip_final(void);
uint8_t		*count_shift(void);
void		shift_to_left_28(uint32_t *c, uint32_t *d, int round);
void		working_pc1(t_des *des);
void		working_pc2(t_des *des, int i);
/*
** File generate_kays.c
*/
void		generate_kays(t_des *des);
void		resize_message(t_des *des);
void		working_file_des(t_des *des);
void		working_input_des(t_des *des);
void		get_message(t_des *des);
/*
** File function_ip.c
*/
uint64_t	function_ip(uint64_t block);
uint64_t	function_e(uint32_t right);
uint8_t		get_number_s(uint64_t val, int i);
uint32_t	function_p(uint32_t val);
uint32_t	function_f(uint32_t right, uint64_t key);
/*
** File function_ip_final.c
*/
uint64_t	function_ip_final(uint64_t block);
void		write_uint64_to_output_message(t_des *des,
				uint64_t block, size_t i);
uint64_t	get_kye(t_des *des, int i);
uint64_t	function_des(t_des *des, uint64_t block64);
void		decode_base64(t_des *des);
/*
** File mode_simple.c
*/
void		mode_simple(t_des *des);
void		mode_des_crt(t_des *des);
void		mode_des_cfb(t_des *des);
void		mode_des_ofb(t_des *des);
void		mode_des_pcbc(t_des *des);
/*
** File mode_des_cbc.c
*/
void		mode_des_cbc(t_des *des);
void		mode_des_ecb(t_des *des);
void		mode_des3(t_des *des);
void		skip_16_octets(t_des *des);
void		run_des(t_des *des);
/*
** File write_to_output_file.c
*/
void		write_to_output_file(t_des *des);
void		encode_base64(t_des *des);
void		add_salt_to_output_message(t_des *des);
void		write_output_message(t_des *des);
t_mode		*get_modes(void);
/*
** File determining_operating_mode.c
*/
void		determining_operating_mode(t_des *des, const char **av);
void		type_des(int ac, const char **av);

#endif
