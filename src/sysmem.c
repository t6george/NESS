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

u8* decodeAddress (u16 address, mainMemory *memory) {
	u8* memPtr = NULL;
	if ((0xE000 & address) == 0x0) {
		memPtr = memory->ram + (0x07FF & address);
	} else if ((0xE000 & address) == 0x2000) {
		memPtr = memory->ppuRegs + (0x0007 & address);
	} else if ((0xFFF0 & address) == 0x4000 || (0xFFF8 & address) == 0x4010) {
		memPtr = memory->apuRegs + (0x001F & address);
	} else if ((0xFFF8 & address) == 0x4018) {
		memPtr = memory->apuIORegs + (0x0007 & address);
	} else {
		memPtr = memory->cartridgeMem + (address - 0x4020);
	}
	return memPtr;
}

u8 readByte (u16 address, addressingMode addrMode, mainMemory *memory, u8 os) {
	u8 *memoryContents = NULL;

	switch (addrMode) {
		case ABSOLUTE: {
			memoryContents = decodeAddress ((u16) (((address & 0xFF) << 8) |
				(address >> 8)), memory);
			break;
		}
		case ZERO_PAGE: {
			memoryContents = decodeAddress (address, memory);
			break;
		}
		case ABSOLUTE_INDEXED: {
			memoryContents = decodeAddress ((u16) ((((address + os) & 0xFF) << 8) |
				((address + os) >> 8)), memory);
			break;
		}
		case ZERO_PAGE_INDEXED: {
			memoryContents = decodeAddress (0xFF & (address + os), memory);
			break;
		}
		case INDEXED_INDIRECT: {
			memoryContents = decodeAddress (((u16)(readByte (0xFF & (address + os), ZERO_PAGE, memory, 0x0))) |
				(((u16)(readByte (0xFF & (address + os + 1), ZERO_PAGE, memory, 0x0))) << 8), memory);
			break;
		}
		case INDIRECT_INDEXED: {
			memoryContents = decodeAddress ((((u16)(readByte (address, ZERO_PAGE, memory, 0x0))) |
				(((u16)(readByte (address + 1, ZERO_PAGE, memory, 0x0))) << 8)) + os, memory);
			break;
		}
		default: {
			break;
		}
	}

	return *memoryContents;
}

void writeByte (u8 data, u16 address, addressingMode addrMode, mainMemory *memory, u8 os) {
	switch (addrMode) {
		case ABSOLUTE: {
			*decodeAddress ((u16) (((address & 0xFF) << 8) |
				(address >> 8)), memory) = data;
			break;
		}
		case ZERO_PAGE: {
			*decodeAddress (address, memory) = data;
			break;
		}
		case ABSOLUTE_INDEXED: {
			*decodeAddress ((u16) ((((address + os) & 0xFF) << 8) |
				((address + os) >> 8)), memory) = data;
			break;
		}
		case ZERO_PAGE_INDEXED: {
			*decodeAddress (0xFF & (address + os), memory) = data;
			break;
		}
		case INDEXED_INDIRECT: {
			*decodeAddress (((u16)(readByte (0xFF & (address + os), ZERO_PAGE, memory, 0x00))) |
				(((u16)(readByte (0xFF & (address + os + 1), ZERO_PAGE, memory, 0x00))) << 8), memory) = data;
			break;
		}
		case INDIRECT_INDEXED: {
			*decodeAddress ((((u16)(readByte (address, ZERO_PAGE, memory, 0x00))) |
				(((u16)(readByte (address + 1, ZERO_PAGE, memory, 0x00))) << 8)) + os, memory) = data;
			break;
		}
		default: {
			break;
		}
	}
}

void powerDownMemory (mainMemory* mem) {
	free ((void*) mem->ram);
	free ((void*) mem->ppuRegs);
	free ((void*) mem->apuRegs);
	free ((void*) mem->apuIORegs);
	free ((void*) mem->cartridgeMem);
}
