cmake_minimum_required(VERSION 3.0)
project(NESEMULATOR)

set (PROJECT_SOURCE_DIR "~/Desktop/NES_Emulator")
set (VERSION 0.1)

configure_file (
	"${PROJECT_SOURCE_DIR}/include/6502.h.in"
	"${PROJECT_BINARY_DIR}/include/6502.h"
  )

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(
	nes
	6502.c
	apu.c
	instructions.c
	ppu.c
	sysmem.c
)
