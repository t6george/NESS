cmake_minimum_required(VERSION 3.0)
set (VERSION 0.1)
project (NES)

file(GLOB_RECURSE NES_SRC "src/*.c")
file(GLOB_RECURSE NES_H "include/*.h")

include_directories(include)

SET(GCC_LINK_FLAGS "-lpthread -lSDL2 -lSDL2main")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")
add_definitions(${GCC_LINK_FLAGS})

configure_file (
  "include/6502.h.in"
  "include/6502.h"
  )

set(SDL2_MORE_INCLUDE_DIR "C:/sdl2")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories(".")

add_executable (nes ${NES_SRC} ${NES_H})
