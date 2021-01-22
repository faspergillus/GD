cmake_minimum_required(VERSION 3.17)
project(hello_lib CXX)

add_library(hello-lib-static STATIC include/greeting.hxx src/greeting.cxx)
target_include_directories(hello-lib-static PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-static PUBLIC cxx_std_17)

add_library(hello-lib-dynamic SHARED include/greeting.hxx src/greeting.cxx)
target_include_directories(hello-lib-dynamic PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-dynamic PUBLIC cxx_std_17)
