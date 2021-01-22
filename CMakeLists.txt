cmake_minimum_required(VERSION 3.17)

project(home_work CXX)

add_subdirectory(hello)
add_subdirectory(simple-hello)

include(CTest)

add_test(NAME check_hello_exit_zero
         COMMAND hello-bin-dynamic)

add_test(NAME check_hello_exit_zero
         COMMAND hello-bin-static)

add_test(NAME check_hello_exit_zero
         COMMAND ex_1_hello_world)
