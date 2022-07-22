#include <err.h>
#include <inttypes.h>
#include <stdio.h>

#include <cuckoo_hash.h>

int main(void)
{
	struct cuckoo *ch = cuckoo_init(16, NULL, NULL, NULL);
	if (ch == NULL)
		err(1, "cuckoo_init");

	printf("n items: %zu\n", ch->nitems);
	printf("max capacity: %zu\n", ch->cap);

	char *key[9] = { "aa", "bb", "cc", "dd", "ee", "ff", "gg", "hh" };
	int val[8] = {1,2,3,4,5,6,7,8};

	for (int i = 0; i < 8; i++) {
		if (cuckoo_insert(ch, key[i], 3, &val[i]) == NULL) {
			printf("Failed to insert %s\n", key[i]);
			continue;
		}
	}

	printf("n items: %zu\n", ch->nitems);

	struct cuckoo_item *last = cuckoo_lookup(ch, key[7], 3);
	if (last) {
		printf("Key: %s\n", (char *)last->key);
		printf("Value: %d\n", *((int *)last->value));
		printf("Hash1: %" PRIx64 "\n", last->hash1);
		printf("Hash2: %" PRIx64 "\n", last->hash2);
	}

	cuckoo_destroy(ch);
	return 0;
}
