#include <stdio.h>
#include <string.h>

#include "../headers/sysmem.h"

mainMemory *initMemory (cpu* mos6502) {
	mainMemory *mem;

	mem->ram = (u8*) malloc (RAM_RANGE);
	mem->ppuRegs (u8*) malloc (PPU_REGS_RANGE);
	mem->apuRegs (u8*) malloc (APU_REGS_RANGE);
	mem->apuIORegs (u8*) malloc (APU_IO_REGS_RANGE);
	mem->cartridgeMem (u8*) malloc (CART_SPACE_RANGE);

	memset ((void*) mem, 0x0, sizeof(mem));

	mem->cpuRegA = mos6502->regA;
	mem->cpuRegX = mos6502->regX;
	mem->cpuRegY = mos6502->regY;
	mem->cpuRegPC = mos6502->regPC;

}

u8* decodeAddress (u16 address, mainMemory *memory) {
	u8* memPtr;
	if (0xE000 & address == 0x0) {
		memPtr = memory->ram + (0x07FF & address);
	} else if (0xE000 & address == 0x2000) {
		memPtr = memory->ppuRegs + (0x0007 & address);
	} else if (0xFFF0 & address == 0x4000 || 0xFFF8 & address == 0x4010) {
		memPtr = memory->apuRegs + (0x001F & address);
	} else if (0xFFF8 & address == 0x4018) {
		memPtr = memory->apuIORegs + (0x0007 & address);
	} else {
		memPtr = memory->cartridgeMem + (address - 0x4020);
	}
	return memPtr;
}

u8 readByte (u16 address, u8 addrMode, mainMemory *memory) {
	u8 *memoryContents = NULL;

	switch (addrMode) {
		case ABSOLUTE: { //account for little endian
			memoryContents = decodeAddress ((u16) (((address & 0xFF) << 8) | (address >> 8)), memory);
			break;
		}
		case ZERO_PAGE: {
			memoryContents = decodeAddress (address, memory);
			break;
		}
		case INDIRECT: {
			memory->cpuRegPC = 
			break;
		}
		case ABSOLUTE_INDEXED: {
			break;
		}
		case ZERO_PAGE_INDEXED: {
			break;
		}
		case INDEXED_INDIRECT: {
			break;
		}
		case INDIRECT_INDEXED: {
			break;
		}
		default: {
			break;
		}
	}
	return *memoryContents;
}

void writeByte (u8 data, u16 address, u8 addrMode, mainMemory *memory) {

}

void uninitMemory (void) {
	free ((void*) mem->ram);
	free ((void*) mem->ppuRegs);
	free ((void*) mem->apuRegs);
	free ((void*) mem->apuIORegs);
	free ((void*) mem->cartridgeMem);
}
