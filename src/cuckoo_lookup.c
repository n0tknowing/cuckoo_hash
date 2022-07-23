#include "cuckoo_hash.h"

struct cuckoo_item *cuckoo_lookup(struct cuckoo *ch, const void *key,
				  size_t len)
{
	if (ch == NULL || key == NULL || len == 0 || ch->nitems == 0)
		return NULL;

	uint64_t hash1 = ch->do_hash1(key, len);
	uint64_t hash2 = ch->do_hash2(key, len);

	ssize_t idx = hash1 % ch->cap;
	struct cuckoo_item *tbl = ch->tbl1[idx];

	if (tbl && tbl->hash1 == hash1 && ch->do_cmp(tbl->key, key, len))
		return tbl;

	idx = hash2 % ch->cap;
	tbl = ch->tbl2[idx];

	if (tbl && tbl->hash2 == hash2 && ch->do_cmp(tbl->key, key, len))
		return tbl;

	return NULL;
}
