add_executable(simple simple.c)
target_link_libraries(simple cuckoo_hash)
target_include_directories(simple
	PRIVATE ${PROJECT_SOURCE_DIR}/include)
