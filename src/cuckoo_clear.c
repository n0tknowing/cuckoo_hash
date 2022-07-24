#include <stdlib.h>

#include "cuckoo_hash.h"

void cuckoo_clear(struct cuckoo *ch)
{
	if (ch == NULL || ch->count == 0)
		return;

	for (ssize_t j = 0; j < ch->capacity; j++) {
		int i = ch->table[0][j].key ? 0 : 1;
		ch->table[i][j].key = NULL;
		ch->table[i][j].value = NULL;
		ch->table[i][j].hash1 = 0;
		ch->table[i][j].hash2 = 0;
	}

	ch->count = 0;
}
