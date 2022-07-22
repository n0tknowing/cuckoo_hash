#include <stdlib.h>
#include <string.h>

#include "cuckoo_hash.h"
#include "hashfunc.h"

static bool cuckoo_cmp(const void *s, const void *p, size_t n)
{
	const char *data1 = s;
	const char *data2 = p;

	return strncmp(data1, data2, n) == 0;
}

struct cuckoo *cuckoo_init(size_t cap, hash_fn h1, hash_fn h2, cmp_fn cmp)
{
	if (cap < CUCKOO_MIN_CAP)
		cap = CUCKOO_MIN_CAP;
	else if (cap > CUCKOO_MAX_CAP)
		cap = CUCKOO_MAX_CAP;
	else if (cap % 2 != 0)
		cap = cap + 1;

	struct cuckoo *ch = malloc(sizeof(struct cuckoo));
	if (ch == NULL)
		return NULL;

	struct cuckoo_item **tbl1 = calloc(cap, sizeof(struct cuckoo_item *));
	if (tbl1 == NULL) {
		free(ch);
		return NULL;
	}

	struct cuckoo_item **tbl2 = calloc(cap, sizeof(struct cuckoo_item *));
	if (tbl2 == NULL) {
		free(tbl1);
		free(ch);
		return NULL;
	}

	for (size_t i = 0; i < cap; i++) {
		tbl1[i] = NULL;
		tbl2[i] = NULL;
	}

	ch->tbl1 = tbl1;
	ch->tbl2 = tbl2;
	ch->nitems = 0;
	ch->cap = cap;
	ch->do_hash1 = h1 ? h1 : fnvhash;
	ch->do_hash2 = h2 ? h2 : murmurhash;
	ch->do_cmp = cmp ? cmp : cuckoo_cmp;

	return ch;
}
