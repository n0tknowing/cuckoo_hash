#include <stdlib.h>

#include "cuckoo_hash.h"

const void *cuckoo_delete(struct cuckoo *ch, const void *key, size_t len)
{
	ssize_t idx = cuckoo_lookup_idx(ch, key, len);
	if (idx == -1)
		return NULL;

	int idxtable = ch->table[0][idx].key ? 0 : 1;
	const void *value = ch->table[idxtable][idx].value;

	ch->table[idxtable][idx].key = NULL;
	ch->table[idxtable][idx].value = NULL;
	ch->table[idxtable][idx].hash1 = 0;
	ch->table[idxtable][idx].hash2 = 0;

	ch->count--;
	return value;
}
