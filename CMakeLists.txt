cmake_minimum_required(VERSION 3.10)

project(cuckoo_hash VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -Wstrict-prototypes -Wmissing-prototypes -Wstrict-aliasing")

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_ASAN "Enable AddressSanitizer" OFF)
option(BUILD_TSAN "Enable ThreadSanitizer" OFF)

if(BUILD_ASAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -fsanitize=address,undefined")
elseif(BUILD_TSAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -fsanitize=thread,undefined")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -march=native")
endif()

add_subdirectory(src)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
