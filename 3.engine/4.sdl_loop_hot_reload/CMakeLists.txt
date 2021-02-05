cmake_minimum_required(VERSION 3.16)
project(sdl_loop_hot_reload CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(engine engine.cxx engine.hxx)
set_target_properties(engine PROPERTIES ENABLE_EXPORTS 1)

find_package(SDL2 REQUIRED)
target_link_libraries(engine PRIVATE SDL2::SDL2)

add_library(game SHARED game.cxx)
target_link_libraries(game PRIVATE engine)
