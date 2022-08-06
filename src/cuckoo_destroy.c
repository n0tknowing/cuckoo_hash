#include <stdlib.h>

#include "cuckoo_hash.h"

void cuckoo_destroy(struct cuckoo *ch)
{
	if (ch == NULL)
		return;

	cuckoo_clear(ch);
	free(ch->table[1]); ch->table[1] = NULL;
	free(ch->table[0]); ch->table[0] = NULL;
	free(ch->table); ch->table = NULL;

	ch->capacity = 0;
	ch->do_hash1 = NULL;
	ch->do_hash2 = NULL;
	ch->do_cmp = NULL;

	free(ch);
}
