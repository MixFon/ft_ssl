#ifndef FT_SSL_H
# define FT_SSL_H

# include <sys/stat.h>
# include "md5.h"
# include "sha256.h"
# include "base64.h"
# include "libft.h"

# define UINT_MAX   0xFFFFFFFF
# define MAX_LEN	0xFFFFFFFFFFFFFFFF

# define P			0
# define Q			1
# define R			2
# define S			3

# define BUFF		4096

typedef struct s_type
{
	void	(*funtion_type)(int ac, const char **av);
	char	*name;
}				t_type;

typedef struct s_alg
{
	t_uchar	*(*algorithm)(const t_uchar *data, size_t count_octets);
	char	*name;
	char	*name_case;
	int		size;
}				t_alg;

typedef struct s_ssl
{
	int		flags[4];
	char	**strings;
	size_t	count_strings;
	char	**file_names;
	size_t	count_file_names;
	t_uchar	*hash;
	size_t	len_message_oct;
	int		i;
	int		k;
	t_alg	*alg;
}			t_ssl;

typedef union u_elem
{
	unsigned int	i_elem;
	unsigned char	c_elem[4];
}					t_elem;

t_alg	*get_algorithms(void);

/*
** File init_ssl.c
*/
void	init_ssl(t_ssl *ssl);
void	help(void);
void	print_usage(t_ssl *ssl, const char c, const char *message);
void	check_valid_flags(t_ssl *ssl, const char *arg);
void	print_array_string(char **arr, size_t len);
/*
** File add_string_to_array.c
*/
void	add_string_to_array(char ***array, size_t *count,
			const char *new_string);
void	add_string(t_ssl *ssl, int ac, const char **av);
int		is_check(t_ssl *ssl, const char *arg);
void	read_flag(t_ssl *ssl, int ac, const char **av);
void	add_file_name(t_ssl *ssl, const char **av, int i);
/*
** File fill_flags.c
*/
void	fill_flags(t_ssl *ssl, int ac, const char **av);
void	check_name_algorithm(t_ssl *ssl, int ac, const char **av);
void	delete_array_string(char ***array, size_t len);
void	deinit_ssl(t_ssl *ssl);
char	*get_data_fd(int fd, size_t *len_message_oct);
/*
** File chenge_endian.c
*/
void	chenge_endian(t_uchar *data, size_t count_octets);
size_t	get_count_zerors(size_t len);
t_uchar	*preparation(const t_uchar *data, size_t *count_octets, t_ssl *ssl);
t_uchar	*get_hash(const t_uchar *data, t_ssl *ssl);
void	print_hash(t_ssl *ssl, const t_uchar *data);
/*
** File working_stdin.c
*/
void	working_stdin(t_ssl *ssl);
void	print_hash_strings_files(t_ssl *ssl, const t_uchar *data,
			const char *info);
void	working_strings(t_ssl *ssl);
int		is_dir(const char *name_file);
void	open_file(t_ssl *ssl, size_t i);
/*
** File working_files.c
*/
void	working_files(t_ssl *ssl);
void	run(t_ssl *ssl);
t_alg	*get_algorithms(void);
int		main(int ac, const char *av[]);
/*
** File type_hashing_algorithms.c
*/
void	type_hashing_algorithms(int ac, const char **av);

#endif
