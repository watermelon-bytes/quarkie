cmake_minimum_required(VERSION 3.0)

project(cheesy VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    ./src/paths.cpp
)

add_library(bitmap
    ${SOURCES}
)

target_include_directories(include)

add_compile_options(
    -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti
    -Wall -Wextra -O2 -g
    -m64 -march=x86-64 -mtls-direct-seg-refs
)
