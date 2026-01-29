cmake_minimum_required(VERSION 3.5)
project(quarkie_hash_test LANGUAGES CXX)

add_executable(hash_function_test
    hash_test.cxx
    ../../src/hash.cxx
)

target_include_directories(hash_function_test PUBLIC ../../inc)
