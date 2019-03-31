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
u8* getPhysAddress (u16 address, mainMemory *memory) {
	u8* physPntr = NULL;
	if ((0xE000 & address) == 0x0) {
		physPntr = memory->ram + (0x07FF & address);
	} else if ((0xE000 & address) == 0x2000) {
		physPntr = memory->ppuRegs + (0x0007 & address);
	} else if ((0xFFF0 & address) == 0x4000 || (0xFFF8 & address) == 0x4010) {
		physPntr = memory->apuRegs + (0x001F & address);
	} else if ((0xFFF8 & address) == 0x4018) {
		physPntr = memory->apuIORegs + (0x0007 & address);
	} else {
		physPntr = memory->cartridgeMem + (address - 0x4020);
	}
	return physPntr;
}

/*Lets refer to the guest's (NES) memory as "virtual"*/
u16 getVirtualAddress (cpu6502 *cpu, addressingMode addrMode, regIndex srcReg, u16 encodedAddr) {
	u16 virtAddress = encodedAddr;
	u8 os = srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[srcReg-1]);
	switch (addrMode) {
		case ABSOLUTE_INDEXED: {
			virtAddress += os;
			break;
		}
		case ZERO_PAGE_INDEXED: {
			virtAddress = 0xFF & (address + os);
			break;
		}
		case INDEXED_INDIRECT: {
			virtAddress = ((u16)*getPhysAddress(0xFF & (virtAddress + os))) |
				(((u16)*getPhysAddress(0xFF & (virtAddress + os + 1))) << 8);
			break;
		}
		case INDIRECT_INDEXED: {
			virtAddress = (((u16)*getPhysAddress(virtAddress)) |
				(((u16)*getPhysAddress(virtAddress + 1)) << 8)) + os;
			break;
		}
		default: {
			break;
		}
		return virtAddress;
}

void powerDownMemory (mainMemory* mem) {
	free ((void*) mem->ram);
	free ((void*) mem->ppuRegs);
	free ((void*) mem->apuRegs);
	free ((void*) mem->apuIORegs);
	free ((void*) mem->cartridgeMem);
}
