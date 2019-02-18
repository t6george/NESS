#include <stdio.h>
#include <string.h>

#include "../headers/sysmem.h"

mainMemory *initMemory (void) {
	mainMemory *mem;
	memset ((void*) mem->ram, 0xFF, RAM_RANGE);
	memset ((void*) mem->apuRegs, 0x00, APU_REGS_RANGE);
	
	memory[0] = ram;
	memory[1] = ppuRegs;
	memory[2] = apuRegs;
	memory[3] = cartridgeMem;
}

u8** decodeAddress (addr address) {
	

}

u8 readByte (addr address, u8 addrMode) {
		
}

void writeByte (u8 data, addr address, u8 addrMode) {

}
