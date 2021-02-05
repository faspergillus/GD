cmake_minimum_required(VERSION 3.17)
project(ex_1_hello_world CXX)

add_executable(ex_1_hello_world main.cpp)
target_compile_features(ex_1_hello_world PUBLIC cxx_std_17)
