cmake_minimum_required(VERSION 3.17)

project(hw1-hello-lib)

add_subdirectory(hello_bin)
add_subdirectory(hello_lib)

#include(CTest)

#add_test(NAME check_hello_exit_zero
#        COMMAND hw1-hello-lib
#         )

