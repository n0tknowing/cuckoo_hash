cmake_minimum_required(VERSION 3.10)

project(cuckoo_hash VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

option(BUILD_EXAMPLES "Build examples" ON)

add_subdirectory(src)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()