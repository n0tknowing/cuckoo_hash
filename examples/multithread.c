#include <err.h>
#include <stdio.h>
#include <pthread.h>
#include <cuckoo_hash.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static const char *key1[6] = {"aa","bb","cc","dd","ee","ff"};
static const char *key2[6] = {"hh","ii","jj","kk","ll","mm"};
static const char *key3[6] = {"hx","xi","rj","ek","jl","nm"};

static const int val1[6] = {1,2,3,4,5,6};
static const int val2[6] = {0,9,8,7,6,5};
static const int val3[6] = {2,4,8,6,0,1};

static void *work1(void *arg)
{
	pthread_mutex_lock(&mtx);

	struct cuckoo *ch = arg;
	for (int i = 0; i < 6; i++) {
		if (cuckoo_insert(ch, key1[i], 3, &val1[i]) == NULL)
			printf("1: failed to insert %s\n", key1[i]);
		else
			printf("1: %s inserted\n", key1[i]);
	}

	pthread_mutex_unlock(&mtx);
	pthread_exit(NULL);

	return NULL;
}

static void *work2(void *arg)
{
	pthread_mutex_lock(&mtx);

	struct cuckoo *ch = arg;
	for (int i = 0; i < 6; i++) {
		if (cuckoo_insert(ch, key2[i], 3, &val2[i]) == NULL)
			printf("2: failed to insert %s\n", key2[i]);
		else
			printf("2: %s inserted\n", key2[i]);
	}

	pthread_mutex_unlock(&mtx);
	pthread_exit(NULL);

	return NULL;
}

static void *work3(void *arg)
{
	pthread_mutex_lock(&mtx);

	struct cuckoo *ch = arg;
	for (int i = 0; i < 6; i++) {
		if (cuckoo_insert(ch, key3[i], 3, &val3[i]) == NULL)
			printf("3: failed to insert %s\n", key3[i]);
		else
			printf("3: %s inserted\n", key3[i]);
	}

	pthread_mutex_unlock(&mtx);
	pthread_exit(NULL);

	return NULL;
}

int main(void)
{
	struct cuckoo *ch = cuckoo_init(16, NULL, NULL, NULL);
	if (ch == NULL)
		err(1, "cuckoo_init");

	printf("capacity: %ld\n", ch->capacity);
	printf("n: %ld\n", ch->count);

	pthread_t thread[3];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if (pthread_create(&thread[0], &attr, work1, ch) != 0)
		err(1, "pthread_create 1");

	if (pthread_create(&thread[1], &attr, work2, ch) != 0)
		err(1, "pthread_create 2");

	if (pthread_create(&thread[2], &attr, work3, ch) != 0)
		err(1, "pthread_create 3");

	for (int i = 0; i < 3; i++) {
		if (pthread_join(thread[i], NULL) != 0)
			printf("%d failed to join\n", i);
	}

	printf("n: %ld\n", ch->count);

	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mtx);
	cuckoo_destroy(ch);
	return 0;
}
