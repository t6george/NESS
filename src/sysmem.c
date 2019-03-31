#include <stdio.h>
#include <string.h>

#include <sysmem.h>

mainMemory* powerUpMemory (void) {
	mainMemory *mem;

	mem->ram = (u8*) malloc (RAM_SIZE);
	mem->ppuRegs = (u8*) malloc (PPU_REGS_SIZE);
	mem->apuRegs = (u8*) malloc (APU_REGS_SIZE);
	mem->apuIORegs = (u8*) malloc (APU_IO_REGS_SIZE);
	mem->cartridgeMem = (u8*) malloc (CART_SPACE_SIZE);

	memset ((void*) mem, 0x0, sizeof(mem));
	return mem;
}

/*Lets refer to the host computer system's memory as "physical"*/
u8* getPhysAddress (mainMemory *memory, u16 virtAddress) {
	u8* physPntr = NULL;
	if ((0xE000 & virtAddress) == 0x0) {
		physPntr = memory->ram + (0x07FF & virtAddress);
	} else if ((0xE000 & virtAddress) == 0x2000) {
		physPntr = memory->ppuRegs + (0x0007 & virtAddress);
	} else if ((0xFFF0 & virtAddress) == 0x4000 || (0xFFF8 & virtAddress) == 0x4010) {
		physPntr = memory->apuRegs + (0x001F & address);
	} else if ((0xFFF8 & virtAddress) == 0x4018) {
		physPntr = memory->apuIORegs + (0x0007 & virtAddress);
	} else {
		physPntr = memory->cartridgeMem + (virtAddress - 0x4020);
	}
	return physPntr;
}

void powerDownMemory (mainMemory* mem) {
	free ((void*) mem->ram);
	free ((void*) mem->ppuRegs);
	free ((void*) mem->apuRegs);
	free ((void*) mem->apuIORegs);
	free ((void*) mem->cartridgeMem);
}
