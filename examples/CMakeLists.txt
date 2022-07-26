set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(simple simple.c)
add_executable(nullvalue nullvalue.c)
add_executable(multithread multithread.c)

target_link_libraries(simple cuckoo_hash)
target_link_libraries(nullvalue cuckoo_hash)
target_link_libraries(multithread cuckoo_hash Threads::Threads)

target_include_directories(simple
	PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(nullvalue
	PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(multithread
	PRIVATE ${PROJECT_SOURCE_DIR}/include)
