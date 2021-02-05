cmake_minimum_required(VERSION 3.16)
project(sdl_static)
set(CMAKE_CXX_STANDARD 17)

add_executable(sdl_static main.cxx)
find_package(SDL2 REQUIRED)
target_include_directories(sdl_static PRIVATE ${SDL2_INCLUDE_DIRS})
find_library(SDL2_LIB
             NAMES libSDL2d.a
             PATHS /usr/local/lib
             )

if(UNIX)
      # find out what libraries are needed for staticaly linking with libSDL.a on Linux
      # using default linux compiler
      # $> sdl2-config --static-libs
      # -lSDL2 -Wl,--no-undefined -lm -ldl -lpthread -lrt

      target_link_libraries(sdl_static
               ${SDL2_LIB} # full path to libSDL2.a force to staticaly link with it
               # -Wl,--no-undefined # not compiled on MacOS
               -lm
               -ldl
               -lpthread
               -lrt
               )
endif()



