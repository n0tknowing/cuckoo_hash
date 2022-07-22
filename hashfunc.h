#ifndef HASHFUNC_H
#define HASHFUNC_H

#include <stdint.h>

#define INLINE	inline __attribute__((always_inline))

static INLINE uint64_t fnvhash(const void *key, size_t len)
{
	const uint8_t *p = (const uint8_t *)key;
	uint64_t hash = 0xcbf29ce484222325ULL;

	for (size_t i = 0; i < len; i++) {
		hash ^= p[i];
		hash *= 0x100000001b3ULL;
	}

	return hash;
}

static INLINE uint64_t murmurhash(const void *key, size_t len)
{
	static const uint64_t m = 0xc6a4a7935bd1e995UL;
	const uint8_t *p = (const uint8_t *)key;
	const uint8_t *end = p + (len & ~0x7);
	uint64_t h = len * m;

	while (p != end) {
		uint64_t k;
		memcpy(&k, p, sizeof(k));

		k *= m;
		k ^= k >> 0x2fUL;
		k *= m;

		h ^= k;
		h *= m;
		p += 0x8;
	}

	switch (len & 0x7) {
	case 7: h ^= (uint64_t)p[6] << 0x30UL; /* fall through */
	case 6: h ^= (uint64_t)p[5] << 0x28UL; /* fall through */
	case 5: h ^= (uint64_t)p[4] << 0x20UL; /* fall through */
	case 4: h ^= (uint64_t)p[3] << 0x18UL; /* fall through */
	case 3: h ^= (uint64_t)p[2] << 0x10UL; /* fall through */
	case 2: h ^= (uint64_t)p[1] << 0x8UL;  /* fall through */
	case 1: h ^= (uint64_t)p[0];           /* fall through */
		h *= m;
	default:
		break;
	};

	h ^= h >> 0x2fUL;
	h *= m;
	h ^= h >> 0x2fUL;

	return h;
}

#endif /* HASHFUNC_H */
