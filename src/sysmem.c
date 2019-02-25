#include <stdio.h>
#include <string.h>

#include "../headers/sysmem.h"

mainMemory *initMemory (void) {
	mainMemory *mem;
	memset ((void*) mem->ram, 0xFF, RAM_RANGE);
	memset ((void*) mem->apuRegs, 0x00, APU_REGS_RANGE);
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

}

void writeByte (u8 data, u16 address, u8 addrMode, mainMemory *memory) {

}
