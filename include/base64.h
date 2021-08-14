#ifndef BASE64_H
# define BASE64_H

# include "libft.h"
# include "ft_ssl.h"

# define UNKNOWN_SYMBOL 0

typedef struct s_base64
{
	int			flags[4];
	char		*chiphertext;
	char		*input_file;
	char		*output_file;
	char		*symbols;
	size_t		size_chiphertext;
}				t_base64;

enum e_flags
{
	base_e,
	base_d,
	base_i,
	base_o
};

void	type_base64(int ac, const char **av);
void	decoding(t_base64 *base, char *data, size_t size);
void	encoding(t_base64 *base, char *data, size_t size);
void	init_base64(t_base64 *base);
void	deinit_base64(t_base64 *base);

/*
** File print_usage_base64.c
*/
void	print_usage_base64(void);
void	print_error_base64(const char *str);
void	fill_name_file(char **name_file, int ac, const char **av, int *i);
void	set_flag(t_base64 *base, int ac, const char **av, int *i);
void	read_flags(t_base64 *base, int ac, const char **av);
/*
** File working_file_base64.c
*/
void	working_file_base64(t_base64 *base, char **data, size_t *read_octets);
void	working_stdin_base64(t_base64 *base, char **data, size_t *read_octets);
void	resize_data(char **data, size_t *size);
void	print_octest(void *data, size_t size);
void	fill_ciphertext(t_base64 *base, uint8_t *numbers);
/*
** File fill_plaintext.c
*/
void	fill_plaintext(t_base64 *base, char *numbers);
void	encoding(t_base64 *base, char *data, size_t size);
int		get_index(t_base64 *base, char c);
void	decoding(t_base64 *base, char *data, size_t size);
void	working_data(t_base64 *base, char *data, size_t size);
/*
** File output_in_file.c
*/
void	output_in_file(t_base64 *base);
void	output_in_console(t_base64 *base);
void	output_chiphertext(t_base64 *base);
void	working_base64(t_base64 *base);
void	deinit_base64(t_base64 *base);
/*
** File init_base64.c
*/
void	init_base64(t_base64 *base);
void	type_base64(int ac, const char **av);

#endif
