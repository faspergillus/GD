cmake_minimum_required(VERSION 3.16)
project(sdl_loop_to_engine_dll CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(engine SHARED engine.cxx engine.hxx)

find_package(SDL2 REQUIRED)
target_link_libraries(engine PRIVATE SDL2::SDL2)

add_executable(game game.cxx)
target_link_libraries(game PRIVATE engine)


