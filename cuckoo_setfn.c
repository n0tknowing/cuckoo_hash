#include "cuckoo_hash.h"

void cuckoo_setfn(struct cuckoo *ch, hash_fn h1, hash_fn h2, cmp_fn cmp)
{
	if (ch == NULL)
		return;

	if (h1)
		ch->do_hash1 = h1;
	if (h2)
		ch->do_hash2 = h2;
	if (cmp)
		ch->do_cmp   = cmp;
}
