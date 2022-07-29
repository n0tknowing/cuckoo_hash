#ifndef CUCKOO_HASH_H
#define CUCKOO_HASH_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#ifndef CUCKOO_MIN_CAP
#define CUCKOO_MIN_CAP	16
#endif

#ifndef CUCKOO_MAX_CAP
#define CUCKOO_MAX_CAP	8192
#endif

#if CUCKOO_MIN_CAP < 0
#error Negative minimum capacity
#endif

typedef bool (*cmp_fn)(const void *, const void *, size_t);
typedef uint64_t (*hash_fn)(const void *, size_t);

struct cuckoo_item {
	const void		*key;
	const void		*value;
	uint64_t		hash1;
	uint64_t		hash2;
};

struct cuckoo {
	struct cuckoo_item	**table;
	ssize_t			capacity;
	ssize_t			count;
	hash_fn			do_hash1;
	hash_fn			do_hash2;
	cmp_fn			do_cmp;
};

struct cuckoo *cuckoo_init(ssize_t, hash_fn, hash_fn, cmp_fn);
ssize_t cuckoo_lookup_idx(struct cuckoo *, const void *, size_t);
struct cuckoo_item *cuckoo_lookup(struct cuckoo *, const void *, size_t);
const void *cuckoo_insert(struct cuckoo *, const void *, size_t, const void *);
const void *cuckoo_delete(struct cuckoo *, const void *, size_t);
void cuckoo_clear(struct cuckoo *);
void cuckoo_destroy(struct cuckoo *);

#endif /* CUCKOO_HASH_H */
