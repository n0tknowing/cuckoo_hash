#include <err.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <cuckoo_hash.h>

static bool intcmp(const void *x, const void *y, size_t n)
{
	(void)n;
	int a = *((int *)x);
	int b = *((int *)y);
	return a == b;
}

int main(void)
{
	struct cuckoo *ch = cuckoo_init(16, NULL, NULL, intcmp);
	if (ch == NULL)
		err(1, "cuckoo_init");

	printf("n items: %zu\n", ch->count);
	printf("max capacity: %zu\n", ch->capacity);

	const int key[8] = {72,91,45,23,12,8888,191919,666666};

	for (int i = 0; i < 8; i++) {
		if (cuckoo_insert(ch, &key[i], sizeof(int), NULL) == NULL) {
			printf("Failed to insert %d\n", key[i]);
			continue;
		}
	}

	printf("n items: %zu\n", ch->count);

	struct cuckoo_item *last = cuckoo_lookup(ch, &key[7], sizeof(int));
	if (last) {
		printf("Key: %d\n", *((int *)last->key));
		printf("Hash1: %" PRIx64 "\n", last->hash1);
		printf("Hash2: %" PRIx64 "\n", last->hash2);
	}

	printf("n items: %zu\n", ch->count);

	const int *v = cuckoo_delete(ch, &key[7], sizeof(int));
	if (v && *v == key[7])
		printf("OK %d\n", *v);
	else
		printf("%d doesn't exist\n", key[7]);

	last = cuckoo_lookup(ch, &key[7], sizeof(int));
	if (last == NULL)
		printf("OK doesn't exist\n");
	else
		printf("WTF??\n");

	cuckoo_destroy(ch);
	return 0;
}
