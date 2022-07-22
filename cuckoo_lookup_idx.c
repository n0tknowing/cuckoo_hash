#include "cuckoo_hash.h"

ssize_t cuckoo_lookup_idx(struct cuckoo *ch, const void *key, size_t len)
{
	if (ch == NULL || key == NULL || len == 0 || ch->nitems == 0)
		return -1;

	uint64_t hash1 = ch->do_hash1(key, len);
	uint64_t hash2 = ch->do_hash2(key, len);

	size_t idx = hash1 % ch->cap;
	struct cuckoo_item *tbl = ch->tbl1[idx];

	if (tbl && tbl->hash1 == hash1 && ch->do_cmp(tbl->key, key, len))
		return (int64_t)idx;

	idx = hash2 % ch->cap;
	tbl = ch->tbl2[idx];

	if (tbl && tbl->hash2 == hash2 && ch->do_cmp(tbl->key, key, len))
		return (int64_t)idx;

	return -1;
}
