#ifndef HMAC_H
# define HMAC_H

# include "ft_ssl.h"

/*
** File get_hash_md5.c
*/
t_uchar	*get_hash_md5(const char *data, size_t size);
t_uchar	*get_key_0(const char *key);
void	get_keypad(t_uchar *key_0, t_uchar *pad, int number);
t_uchar	*concat_two_string(t_uchar *one, size_t size_one,
			t_uchar *two, size_t size_two);
t_uchar	*get_hmac(t_uchar *ikeypad, t_uchar *okeypad, const t_uchar *salt);
/*
** File hmac_md5.c
*/
t_uchar	*hmac_md5(const char *key, const t_uchar *salt);

#endif
