#include <stdlib.h>

#include "cuckoo_hash.h"

void cuckoo_destroy(struct cuckoo *ch)
{
	if (ch == NULL)
		return;

	cuckoo_clear(ch);
	free(ch->table[1]);
	free(ch->table[0]);
	free(ch->table);

	ch->capacity = 0;
	ch->do_hash1 = NULL;
	ch->do_hash2 = NULL;
	ch->do_cmp = NULL;
	ch->table = NULL;

	free(ch);
}
