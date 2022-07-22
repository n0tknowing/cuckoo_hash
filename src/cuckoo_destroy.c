#include <stdlib.h>

#include "cuckoo_hash.h"

void cuckoo_destroy(struct cuckoo *ch)
{
	if (ch == NULL)
		return;

	cuckoo_clear(ch);
	free(ch->tbl1); ch->tbl1 = NULL;
	free(ch->tbl2); ch->tbl2 = NULL;

	ch->cap       = 0;
	ch->do_hash1  = NULL;
	ch->do_hash2  = NULL;
	ch->do_cmp    = NULL;

	free(ch);
}
