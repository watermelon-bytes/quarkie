cmake_minimum_required(VERSION 3.5)
project(quarkie_test_bitmap_search)

add_executable( test_bitmap_search main.cpp ../../src/hash.cxx ../../src/node.cxx )
target_include_directories(test_bitmap_search PUBLIC ../../inc/)
