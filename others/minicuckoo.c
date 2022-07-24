#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
	char *key;
	void *value;
	bool occupied;
} hashnode;

typedef struct {
	hashnode *node;
	ssize_t count;
	ssize_t cap;
} hashtable;

static inline ssize_t hashfunc1(char *key)
{
	ssize_t res = 0x1505;
	size_t l = strlen(key);

	while (l--)
		res = ((res << 5)) ^ *key++;

	return res;
}

static inline ssize_t hashfunc2(char *key)
{
	ssize_t res = 0x811c9dc5;
	size_t l = strlen(key);

	for (size_t i = 0; i < l; i++) {
		res ^= key[i];
		res *= 0x1000193;
	}

	return res;
}

int init(hashtable *h, ssize_t cap)
{
	cap = cap % 2 ? cap * 2 + 1 : cap * 2;
	cap = cap < 64 ? 64 : cap > 131072 ? 131072 : cap;

	hashnode *node = calloc(cap, sizeof(hashnode));
	if (node == NULL)
		return -1;

	h->node = node;
	h->count = 0;
	h->cap = cap;

	return 0;
}

ssize_t lookup(hashtable *h, char *key)
{
	hashnode *node = h->node;
	ssize_t idx, hash, cap = h->cap >> 1;

	hash = hashfunc1(key);
	idx = hash % cap;
	if (node[idx].occupied && !strcmp(key, node[idx].key))
		return idx;

	hash = hashfunc2(key);
	idx = hash % cap;
	idx = idx + cap - 1;
	if (node[idx].occupied && !strcmp(key, node[idx].key))
		return idx;

	return -1;
}

ssize_t insert(hashtable *h, char *key, void *value)
{
	char *tk, *tv;
	ssize_t (*hashfunc)(char *) = hashfunc1;
	ssize_t idx, hash, cap = h->cap >> 1, maxloop = cap >> 1;

	idx = lookup(h, key);
	if (idx != -1)
		return idx;

	while (maxloop > 0) {
		hash = hashfunc(key);
		idx = hash % cap;
		if (hashfunc == hashfunc2)
			idx += cap - 1;

		if (h->node[idx].occupied == false) {
			h->node[idx].key = key;
			h->node[idx].value = value;
			h->node[idx].occupied = true;
			h->count++;
			return idx;
		}

		tk = h->node[idx].key;
		tv = h->node[idx].value;
		h->node[idx].key = key;
		h->node[idx].value = value;
		key = tk;
		value = tv;

		if (hashfunc == hashfunc1) {
			hashfunc = hashfunc2;
			continue;
		}

		hashfunc = hashfunc1;
		maxloop--;
	}

	return -1;
}

void delete(hashtable *h, char *key)
{
	ssize_t idx = lookup(h, key);
	if (idx == -1)
		return;

	h->node[idx].occupied = false;
	h->count--;
}

void clear(hashtable *h)
{
	for (ssize_t i = 0; i < h->cap; i++)
		h->node[i].occupied = false;

	h->count = 0;
}

void destroy(hashtable *h)
{
	h->cap = 0;
	free(h->node);
	h->node = NULL;
}

int main(void)
{
	hashtable ht, *h;
	if (init(&ht, 8192) == -1)
		return 1;

	h = &ht;
	printf("cap: %zu, table cap: %zu\n", h->cap, h->cap >> 1);

	ssize_t idx[10];
	char *key[10] = {"aa","bb","cc","dd","ee","ff","gg","hh","ii","jj"};
	int val[10] = {1,2,3,4,5,6,7,8,9,0};

	for (int i = 0; i < 10; i++) {
		idx[i] = insert(h, key[i], &val[i]);
		if (idx[i] == -1)
			printf("%s failed\n", key[i]);
	}

	printf("%zu\n", h->count);

	for (int i = 0; i < 10; i++) {
		ssize_t _idx = lookup(h, key[i]);
		if (_idx == idx[i])
			printf("%s OK\n", key[i]);
	}

	for (int i = 0; i < 5; i++)
		delete(h, key[i]);

	printf("%zu\n", h->count);
	clear(h);
	printf("%zu\n", h->count);

	destroy(h);
	return 0;
}
