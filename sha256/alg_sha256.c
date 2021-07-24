#include "../include/sha256.h"

t_uchar	*alg_sha256(const t_uchar *data, const size_t count_octets)
{
	t_uchar	*result;
	t_sha	sha;

	init_sha256(&sha, data, count_octets);
	working_sha256(&sha);
	result = get_string_hash(sha.hash, LEN_HASH_SHA256);
	chenge_endian(result, LEN_HASH_SHA256);
	return (result);
}
