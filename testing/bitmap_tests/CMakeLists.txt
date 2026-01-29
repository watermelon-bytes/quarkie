cmake_minimum_required(VERSION 3.5)
project(quarkie-tests LANGUAGES CXX)


add_executable(pool_tests pool_tests.cxx)
target_include_directories(pool_tests PUBLIC ../inc/)

