cmake_minimum_required(VERSION 3.16)
project(sdl_dynamic)
set(CMAKE_CXX_STANDARD 17)

add_executable(sdl_dynamic main.cxx)
find_package(SDL2 REQUIRED)
target_link_libraries(sdl_dynamic PRIVATE SDL2::SDL2 SDL2::SDL2main)
