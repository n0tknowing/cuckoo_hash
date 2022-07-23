#include <stdlib.h>

#include "cuckoo_hash.h"

void cuckoo_clear(struct cuckoo *ch)
{
	if (ch == NULL || ch->nitems == 0)
		return;

	for (ssize_t i = 0; i < ch->cap; i++) {
		if (ch->tbl1[i]) {
			ch->tbl1[i]->key   = NULL;
			ch->tbl1[i]->value = NULL;
			ch->tbl1[i]->hash1 = 0;
			ch->tbl1[i]->hash2 = 0;
			free(ch->tbl1[i]);
			ch->tbl1[i] = NULL;
		}

		if (ch->tbl2[i]) {
			ch->tbl2[i]->key   = NULL;
			ch->tbl2[i]->value = NULL;
			ch->tbl2[i]->hash1 = 0;
			ch->tbl2[i]->hash2 = 0;
			free(ch->tbl2[i]);
			ch->tbl2[i] = NULL;
		}
	}

	ch->nitems = 0;
}
