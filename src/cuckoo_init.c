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

struct cuckoo *cuckoo_init(ssize_t cap, hash_fn h1, hash_fn h2, cmp_fn cmp)
{
	if (cap % 2 != 0)
		cap = cap + 1;

	if (cap < CUCKOO_MIN_CAP || cap > CUCKOO_MAX_CAP)
		cap = CUCKOO_MIN_CAP;

	struct cuckoo *ch = malloc(sizeof(struct cuckoo));
	if (ch == NULL)
		return NULL;

	struct cuckoo_item **table = malloc(sizeof(struct cuckoo_item) * 2);
	if (table == NULL) {
		free(ch);
		return NULL;
	}

	if ((table[0] = calloc(cap, sizeof(struct cuckoo_item))) == NULL) {
		free(table);
		free(ch);
		return NULL;
	}

	if ((table[1] = calloc(cap, sizeof(struct cuckoo_item))) == NULL) {
		free(table[0]);
		free(table);
		free(ch);
		return NULL;
	}

	ch->table = table;
	ch->count = 0;
	ch->capacity = cap;
	ch->do_hash1 = h1 ? h1 : fnvhash;
	ch->do_hash2 = h2 ? h2 : murmurhash;
	ch->do_cmp = cmp ? cmp : cuckoo_cmp;

	return ch;
}
