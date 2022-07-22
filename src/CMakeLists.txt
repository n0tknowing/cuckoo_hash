set(CMAKE_C_VISIBILITY_PRESET hidden)

add_library(cuckoo_hash STATIC
	cuckoo_clear.c
	cuckoo_delete.c
	cuckoo_destroy.c
	cuckoo_init.c
	cuckoo_insert.c
	cuckoo_lookup.c
	cuckoo_lookup_idx.c
	cuckoo_setfn.c)

target_include_directories(cuckoo_hash
	PUBLIC ${PROJECT_SOURCE_DIR}/include)
