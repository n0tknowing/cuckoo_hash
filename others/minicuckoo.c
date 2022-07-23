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
	size_t count;
	size_t cap;
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

hashtable *init(size_t cap)
{
	if (cap < 16)
		cap = 16;
	else if (cap > 8192)
		cap = 8192;
	else if (cap % 2 != 0)
		cap = cap + 1;

	hashtable *h = malloc(sizeof(hashtable));
	if (h == NULL)
		return NULL;

	hashnode *node = calloc(cap, sizeof(hashnode));
	if (node == NULL) {
		free(h);
		return NULL;
	}

	for (size_t i = 0; i < cap; i++)
		node[i].occupied = false;

	h->node = node;
	h->count = 0;
	h->cap = cap;
	return h;
}

ssize_t lookup(hashtable *h, char *key)
{
	hashnode *node = h->node;
	ssize_t index, hash, cap = h->cap >> 1;

	// first table
	hash = hashfunc1(key);
	index = hash % cap;
	if (node[index].occupied && !strcmp(key, node[index].key))
		return index;

	// second table
	hash = hashfunc2(key);
	index = hash % cap;
	index = index + cap - 1;
	if (node[index].occupied && !strcmp(key, node[index].key))
		return index;

	return -1;
}

ssize_t insert(hashtable *h, char *key, void *value)
{
	char *tk, *tv;
	ssize_t index, hash, cap = h->cap >> 1, maxloop = cap >> 1;
	index = lookup(h, key);
	if (index != -1)
		return index;

	while (maxloop > 0) {
		hash = hashfunc1(key);
		index = hash % cap;
		if (h->node[index].occupied == false) {
			h->node[index].key = key;
			h->node[index].value = value;
			h->node[index].occupied = true;
			h->count++;
			return index;
		}

		tk = h->node[index].key;
		tv = h->node[index].value;
		h->node[index].key = key;
		h->node[index].value = value;
		key = tk;
		value = tv;

		hash = hashfunc2(key);
		index = hash % cap;
		index = index + cap - 1;
		if (h->node[index].occupied == false) {
			h->node[index].key = key;
			h->node[index].value = value;
			h->node[index].occupied = true;
			h->count++;
			return index;
		}

		tk = h->node[index].key;
		tv = h->node[index].value;
		h->node[index].key = key;
		h->node[index].value = value;
		key = tk;
		value = tv;

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
	for (size_t i = 0; i < h->cap; i++)
		h->node[i].occupied = false;

	h->count = 0;
}

void destroy(hashtable *h)
{
	h->cap = 0;
	free(h->node);
	h->node = NULL;
	free(h);
}

int main(void)
{
	hashtable *h = init(8192);
	if (h == NULL)
		return 1;

	ssize_t idx[10];
	char *key[11] = {"aa","bb","cc","dd","ee","ff","gg","hh","ii","jj"};
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
