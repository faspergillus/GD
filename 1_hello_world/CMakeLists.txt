cmake_minimum_required(VERSION 3.17)

project(home_work CXX)

add_subdirectory(hello-lib)
add_subdirectory(simple-hello)

include(CTest)

add_test(NAME check_hello-bin-dynamic
         COMMAND hello-bin-dynamic)

add_test(NAME check_hello-bin-static
         COMMAND hello-bin-static)

add_test(NAME check_hello_world
         COMMAND ex_1_hello_world)
