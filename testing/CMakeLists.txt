cmake_minimum_required(VERSION 3.5)
add_executable(hash_function_test
    hash-test.cxx
    ../src/hash.cxx)

target_include_directories(hash_function_test PUBLIC ../inc)
add_compile_options(-O)
