cmake_minimum_required(VERSION 3.16)
project(sdl_loop_to_engine CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(sdl_loop_to_engine main.cxx engine.cxx engine.hxx)
find_package(SDL2 REQUIRED)
target_link_libraries(sdl_loop_to_engine PRIVATE SDL2::SDL2-static)
