cmake_minimum_required(VERSION 3.10)

project(QuarkieFS VERSION 0.0.1 LANGUAGES CXX)

# set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) # <-- dont know whether it's necessary or not

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QUARKIE_SOURCES
    src/paths.cxx
    src/node.cxx
    src/superblock.cxx
    src/free_space_tracker.cxx
)

add_library(quarkie_api STATIC ${QUARKIE_SOURCES})

target_include_directories(quarkie_api PUBLIC inc/)


add_compile_options(quarkie_api PRIVATE # do not export compile options
    -nostdlib -nostdinc++
    -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti
    # hope this ^^^^ line will setup full x86-64 freestanding
    -Wall -Wextra -O1 -g
    -m64 -march=x86-64 -mtls-direct-seg-refs
)

#add_subdirectory(testing/bitmap_tests/)
#add_subdirectory(testing/hash_function_tests/)
#add_subdirectory(testing/bitmap_search_function_tests/)
