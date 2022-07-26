cmake_minimum_required(VERSION 3.10)

project(cuckoo_hash LANGUAGES C)

set(CUCKOO_MAJOR_VERSION 1)
set(CUCKOO_MINOR_VERSION 1)
set(CUCKOO_PATCH_VERSION 0)
set(CUCKOO_VERSION ${CUCKOO_MAJOR_VERSION}.${CUCKOO_MINOR_VERSION}.${CUCKOO_PATCH_VERSION})

include(GNUInstallDirs)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -Wstrict-prototypes -Wmissing-prototypes -Wstrict-aliasing")

option(CUCKOO_BUILD_SHARED "Build shared library" ON)
option(CUCKOO_BUILD_STATIC "Build static library" OFF)
option(CUCKOO_BUILD_EXAMPLE "Build example" ON)
option(CUCKOO_ENABLE_ASAN "Enable AddressSanitizer (debugging)" OFF)
option(CUCKOO_ENABLE_TSAN "Enable ThreadSanitizer (debugging)" OFF)

set(CUCKOO_SRC
	src/cuckoo_clear.c
	src/cuckoo_delete.c
	src/cuckoo_destroy.c
	src/cuckoo_init.c
	src/cuckoo_insert.c
	src/cuckoo_lookup.c
	src/cuckoo_lookup_idx.c)

if(CUCKOO_ENABLE_ASAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -fsanitize=address,undefined")
elseif(CUCKOO_ENABLE_TSAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -fsanitize=thread,undefined")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

if(NOT DEFINED CUCKOO_MIN_CAP)
	set(CUCKOO_MIN_CAP 16)
endif()

if(NOT DEFINED CUCKOO_MAX_CAP)
	set(CUCKOO_MAX_CAP 8192)
endif()

if(CUCKOO_MIN_CAP LESS 0)
	message(FATAL_ERROR "Negative minimum capacity")
elseif(CUCKOO_MAX_CAP LESS 0)
	message(FATAL_ERROR "Negative maximum capacity")
endif()

if(CUCKOO_MAX_CAP LESS_EQUAL CUCKOO_MIN_CAP)
	message(FATAL_ERROR "Maximum capacity must greater than minimum capacity")
endif()

add_definitions(-DCUCKOO_MIN_CAP=${CUCKOO_MIN_CAP})
add_definitions(-DCUCKOO_MAX_CAP=${CUCKOO_MAX_CAP})

if(CUCKOO_BUILD_STATIC)
	add_library(cuckoo_hash STATIC ${CUCKOO_SRC})
	message(STATUS "-- Building static library")
elseif(CUCKOO_BUILD_SHARED)
	add_library(cuckoo_hash SHARED ${CUCKOO_SRC})
	message(STATUS "-- Building shared library")
else()
	message(FATAL_ERROR "CUCKOO_BUILD_SHARED and CUCKOO_BUILD_STATIC is disabled")
endif()

message(STATUS "-- C standard: C${CMAKE_C_STANDARD}")
message(STATUS "-- CFLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "-- CUCKOO_MIN_CAP: ${CUCKOO_MIN_CAP}")
message(STATUS "-- CUCKOO_MAX_CAP: ${CUCKOO_MAX_CAP}")

target_include_directories(cuckoo_hash PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(cuckoo_hash PROPERTIES
	VERSION ${CUCKOO_VERSION}
	SOVERSION ${CUCKOO_MAJOR_VERSION}.${CUCKOO_MINOR_VERSION})

if(CUCKOO_BUILD_EXAMPLE)
	add_subdirectory(examples)
endif()

set(includedir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(prefix ${CMAKE_INSTALL_PREFIX})
configure_file(libcuckoo_hash.pc.in libcuckoo_hash.pc @ONLY)

install(FILES ${PROJECT_BINARY_DIR}/libcuckoo_hash.pc
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
install(DIRECTORY include/
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS cuckoo_hash
	EXPORT cuckoo_hash
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
