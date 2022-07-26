#include <stdlib.h>

#include "cuckoo_hash.h"

static inline void swap(struct cuckoo_item *x, struct cuckoo_item *y)
{
	struct cuckoo_item tmp = *x;
	*x = *y;
	*y = tmp;
}

const void *cuckoo_insert(struct cuckoo *ch, const void *key, size_t len,
			  const void *value)
{
	if (ch == NULL || key == NULL || len == 0)
		return NULL;

	if (ch->capacity == 0) {
		hash_fn hf[2] = { ch->do_hash1, ch->do_hash2 };
		cmp_fn cmpf = ch->do_cmp;
		cuckoo_destroy(ch);
		ch = cuckoo_init(CUCKOO_MIN_CAP, hf[0], hf[1], cmpf);
		if (ch == NULL)
			return NULL;
	} else if (cuckoo_lookup(ch, key, len)) {
		return value;
	}

	uint64_t hash1 = ch->do_hash1(key, len);
	uint64_t hash2 = ch->do_hash2(key, len);
	ssize_t max_iter = ch->capacity >> 1, idx;
	struct cuckoo_item tmp = { key, value, hash1, hash2 };

	while (max_iter > 0) {
		idx = tmp.hash1 % ch->capacity;
		if (ch->table[0][idx].key == NULL) {
			ch->table[0][idx].key = tmp.key;
			ch->table[0][idx].value = tmp.value;
			ch->table[0][idx].hash1 = tmp.hash1;
			ch->table[0][idx].hash2 = tmp.hash2;
			ch->count++;
			return value ? value : key;
		}

		swap(&tmp, &ch->table[0][idx]);

		idx = tmp.hash2 % ch->capacity;
		if (ch->table[1][idx].key == NULL) {
			ch->table[1][idx].key = tmp.key;
			ch->table[1][idx].value = tmp.value;
			ch->table[1][idx].hash1 = tmp.hash1;
			ch->table[1][idx].hash2 = tmp.hash2;
			ch->count++;
			return value ? value : key;
		}

		swap(&tmp, &ch->table[1][idx]);
		max_iter--;
	}

	/* resize? */
	/* rehash? */
	/* insert again? */
	return NULL;
}
