#include <stdlib.h>

#include "cuckoo_hash.h"

const void *cuckoo_insert(struct cuckoo *ch, const void *key, size_t len,
			  const void *value)
{
	if (ch == NULL || key == NULL || len == 0 || value == NULL)
		return NULL;

	struct cuckoo_item *tmp = cuckoo_lookup(ch, key, len);
	if (tmp)
		return tmp->value;

	uint64_t hash1 = ch->do_hash1(key, len);
	uint64_t hash2 = ch->do_hash2(key, len);
	size_t max_iter = ch->cap >> 1;  /* ??? */

	struct cuckoo_item *item = malloc(sizeof(struct cuckoo_item));
	if (item == NULL)
		return NULL;

	/* Shallow copy */
	item->key = key;
	item->value = value;
	item->hash1 = hash1;
	item->hash2 = hash2;

	while (max_iter > 0) {
		size_t idx = item->hash1 % ch->cap;
		if (ch->tbl1[idx] == NULL) {
			ch->tbl1[idx] = item;
			ch->nitems++;
			return value;
		}

		/* Swap */
		tmp = ch->tbl1[idx];
		ch->tbl1[idx] = item;
		item = tmp;

		/* Now 'item' holds old item from first table */

		idx = item->hash2 % ch->cap;
		if (ch->tbl2[idx] == NULL) {
			/* Insert old 'item' into second table */
			ch->tbl2[idx] = item;
			ch->nitems++;
			return value;
		}

		/* Swap */
		tmp = ch->tbl2[idx];
		ch->tbl2[idx] = item;
		item = tmp;

		max_iter--;
	}

	/* rehash? */
	/* insert again? */
	return NULL;
}
