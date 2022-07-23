#include "cuckoo_hash.h"

struct cuckoo_item *cuckoo_lookup(struct cuckoo *ch, const void *key,
				  size_t len)
{
	if (ch == NULL || key == NULL || len == 0 || ch->nitems == 0)
		return NULL;

	uint64_t hash = ch->do_hash1(key, len);
	ssize_t idx = hash % ch->cap;
	struct cuckoo_item *tbl = ch->tbl1[idx];

	if (tbl && tbl->hash1 == hash && ch->do_cmp(tbl->key, key, len))
		return tbl;

	hash = ch->do_hash2(key, len);
	idx = hash % ch->cap;
	tbl = ch->tbl2[idx];

	if (tbl && tbl->hash2 == hash && ch->do_cmp(tbl->key, key, len))
		return tbl;

	return NULL;
}
