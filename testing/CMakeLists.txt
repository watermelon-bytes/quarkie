cmake_minimum_required(VERSION 3.0)

add_executable(bitmap-test.out
    bitmap-test2.cpp
)

add_executable(test_parser.out
    test_parser.cpp
    ../src/paths.cxx
)

target_include_directories(test_parser.out PUBLIC ../inc)
add_compile_options(-O2)
