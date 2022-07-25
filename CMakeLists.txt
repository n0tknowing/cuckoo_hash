cmake_minimum_required(VERSION 3.10)

project(cuckoo_hash LANGUAGES C)

set(CUCKOO_MAJOR_VERSION 1)
set(CUCKOO_MINOR_VERSION 0)
set(CUCKOO_PATCH_VERSION 0)
set(CUCKOO_VERSION ${CUCKOO_MAJOR_VERSION}.${CUCKOO_MINOR_VERSION}.${CUCKOO_PATCH_VERSION})

include(GNUInstallDirs)
include(CheckCSourceCompiles)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD 11)
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

check_c_source_compiles("#include <stdatomic.h>
int main(void) {
  _Atomic int a = 0;
  atomic_int aa = 0;
  return a + aa;
}" CUCKOO_HAVE_STDATOMIC)

if(NOT CUCKOO_HAVE_STDATOMIC)
	message(FATAL_ERROR "stdatomic not working on this machine, exiting...")
endif()

add_definitions(-DCUCKOO_MIN_CAP=${CUCKOO_MIN_CAP})
add_definitions(-DCUCKOO_MAX_CAP=${CUCKOO_MAX_CAP})

if(CUCKOO_BUILD_STATIC)
	add_library(cuckoo_hash STATIC ${CUCKOO_SRC})
elseif(CUCKOO_BUILD_SHARED)
	add_library(cuckoo_hash SHARED ${CUCKOO_SRC})
else()
	message(FATAL_ERROR "CUCKOO_BUILD_SHARED and CUCKOO_BUILD_STATIC is disabled")
endif()

target_include_directories(cuckoo_hash PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(cuckoo_hash PROPERTIES
	VERSION ${CUCKOO_VERSION}
	SOVERSION ${CUCKOO_MAJOR_VERSION}.${CUCKOO_MINOR_VERSION})

if(CUCKOO_BUILD_EXAMPLE)
	add_subdirectory(examples)
endif()

install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS cuckoo_hash
	EXPORT cuckoo_hash
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
