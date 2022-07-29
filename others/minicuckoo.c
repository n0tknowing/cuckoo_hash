#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ISPOWOF2(a)	((a) > 0 && !((a) & ((a) - 1)))
#define GETIDX(a, n)	((a) & ((n) - 1))
#define GETIDX2(a, n)	(GETIDX(a, n) + (n) - 1)

typedef struct {
	char *key;
	void *value;
	bool occupied;
} hashnode;

typedef struct {
	hashnode *node;
	int count;
	int cap;
} hashtable;

static inline unsigned int hashfunc1(char *key)
{
	unsigned int res = 0x1505U;
	size_t l = strlen(key);

	while (l--)
		res = ((res << 5U)) ^ *key++;

	return res;
}

static inline unsigned int hashfunc2(char *key)
{
	unsigned int res = 0x811c9dc5U;
	size_t l = strlen(key);

	for (size_t i = 0; i < l; i++) {
		res ^= key[i];
		res *= 0x1000193U;
	}

	return res;
}

int init(hashtable *h, unsigned int cap)
{
	cap = cap < 64 ? 64 : cap > INT_MAX ? INT_MAX : (int)cap;
	if (!ISPOWOF2(cap))
		return -1;

	hashnode *node = calloc(cap, sizeof(hashnode));
	if (node == NULL)
		return -1;

	h->node = node;
	h->count = 0;
	h->cap = cap;

	return 0;
}

int lookup(hashtable *h, char *key)
{
	hashnode *node = h->node;
	unsigned int hash;
	int idx, cap = h->cap >> 1;

	hash = hashfunc1(key);
	idx = GETIDX(hash, cap);
	if (node[idx].occupied && !strcmp(key, node[idx].key))
		return idx;

	hash = hashfunc2(key);
	idx = GETIDX2(hash, cap);
	if (node[idx].occupied && !strcmp(key, node[idx].key))
		return idx;

	return -1;
}

int insert(hashtable *h, char *key, void *value)
{
	char *tk, *tv;
	unsigned int (*hashfunc)(char *) = hashfunc1;
	unsigned int hash;
	int idx, cap = h->cap >> 1, maxloop = cap >> 1;

	idx = lookup(h, key);
	if (idx != -1)
		return idx;

	while (maxloop > 0) {
		hash = hashfunc(key);
		if (hashfunc == hashfunc1)
			idx = GETIDX(hash, cap);
		else
			idx = GETIDX2(hash, cap);

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
	int idx = lookup(h, key);
	if (idx == -1)
		return;

	h->node[idx].occupied = false;
	h->count--;
}

void clear(hashtable *h)
{
	for (int i = 0; i < h->cap; i++)
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
	printf("cap: %d, table cap: %d\n", h->cap, h->cap >> 1);

	int idx[10];
	char *key[10] = {"aa","bb","cc","dd","ee","ff","gg","hh","ii","jj"};
	int val[10] = {1,2,3,4,5,6,7,8,9,0};

	for (int i = 0; i < 10; i++) {
		idx[i] = insert(h, key[i], &val[i]);
		if (idx[i] == -1)
			printf("%s failed\n", key[i]);
	}

	printf("%d\n", h->count);

	for (int i = 0; i < 10; i++) {
		int _idx = lookup(h, key[i]);
		if (_idx != -1 && _idx == idx[i])
			printf("%s OK (%d)\n", key[i], idx[i]);
	}

	for (int i = 0; i < 5; i++)
		delete(h, key[i]);

	printf("%d\n", h->count);
	clear(h);
	printf("%d\n", h->count);

	destroy(h);
	return 0;
}
