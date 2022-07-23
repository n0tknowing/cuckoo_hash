#include <stdlib.h>

#include "cuckoo_hash.h"

const void *cuckoo_delete(struct cuckoo *ch, const void *key, size_t len)
{
	ssize_t idx = cuckoo_lookup_idx(ch, key, len);
	if (idx == -1)
		return NULL;

	const void *value = NULL;
	if (ch->tbl1[idx]) {
		value = ch->tbl1[idx]->value;
		ch->tbl1[idx]->key = NULL;
		ch->tbl1[idx]->value = NULL;
		ch->tbl1[idx]->hash1 = 0;
		ch->tbl1[idx]->hash2 = 0;
		free(ch->tbl1[idx]);
		ch->tbl1[idx] = NULL;
	} else if (ch->tbl2[idx]) {
		value = ch->tbl2[idx]->value;
		ch->tbl2[idx]->key = NULL;
		ch->tbl2[idx]->value = NULL;
		ch->tbl2[idx]->hash1 = 0;
		ch->tbl2[idx]->hash2 = 0;
		free(ch->tbl2[idx]);
		ch->tbl2[idx] = NULL;
	}

	ch->nitems--;
	return value;
}
