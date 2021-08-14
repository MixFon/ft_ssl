#include "../include/hmac.h"

t_uchar	*hmac_md5(const char *key, const t_uchar *salt)
{
	t_uchar	*key_0;
	t_uchar	ikeypad[SIZE_MD5];
	t_uchar	okeypad[SIZE_MD5];

	key_0 = get_key_0(key);
	get_keypad(key_0, ikeypad, 0x36);
	get_keypad(key_0, okeypad, 0x5c);
	free(key_0);
	return (get_hmac(ikeypad, okeypad, salt));
}
