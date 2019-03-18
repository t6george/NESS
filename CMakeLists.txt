cmake_minimum_required(VERSION 3.0)
set (VERSION 0.1)
project (NES)

file(GLOB_RECURSE NES_SRC "src/*.c")
file(GLOB_RECURSE NES_H "include/*.h")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lSDL2 -lSDL2main")

configure_file (
  "include/6502.h.in"
  "include/6502.h"
  )


find_package(SDL2 REQUIRED)
include_directories(include ${SDL2_INCLUDE_DIRS})
add_executable (nes ${NES_SRC} ${NES_H})
target_link_libraries(nes ${SDL2_LIBRARIES})
