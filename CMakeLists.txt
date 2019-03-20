cmake_minimum_required(VERSION 3.10)

set (VERSION 0.1)
project (NES)

file(GLOB_RECURSE NES_SRC "src/*.c")
file(GLOB_RECURSE NES_H "include/*.h")
include_directories(include)

configure_file (
  "include/6502.h.in"
  "include/6502.h"
  )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIRS})

add_executable(NES WIN32 ${NES_SRC} ${NES_H})
target_link_libraries(NES ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
