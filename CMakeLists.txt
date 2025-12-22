cmake_minimum_required(VERSION 3.0)

project(QuarkieFS VERSION 0.0.1 LANGUAGES CXX)

# set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True) # <-- dont know whether it's necessary or not

set(QUARKIE_SOURCES
    src/paths.cpp
    src/node.cxx
    src/superblock.cpp
    src/free_space_tracker.cpp
)

add_library(quarkie_api STATIC ${QUARKIE_SOURCES})
target_include_directories(quarkie_api  [AFTER|BEFORE]
   <INTERFACE|PUBLIC|PRI...> [items1...]
   [<INTERFACE|PUBLIC|PRI...> [items2...] ...])
add_compile_options(quarkie_api PRIVATE # do not export compile options
    -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti
    # hope this ^^^^ will setup full x86-64 freestanding
    #
    -Wall -Wextra -O1 -g
    -m64 -march=x86-64 -mtls-direct-seg-refs
)
