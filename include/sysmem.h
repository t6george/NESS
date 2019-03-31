#pragma once
#include <utils.h>

#define RAM_SIZE 0x800
#define PPU_REGS_SIZE 0x8
#define APU_IO_REGS_SIZE 0x8
#define APU_REGS_SIZE 0x18
#define CART_SPACE_SIZE 0xBFE0

typedef struct {
	u8 *ram;
	u8 *ppuRegs;
	u8 *apuRegs;
	u8 *apuIORegs;
	u8 *cartridgeMem;

} mainMemory;

typedef enum {
	NON_MEMORY = 0,
	ABSOLUTE,
	ZERO_PAGE,
	ABSOLUTE_INDEXED,
	ZERO_PAGE_INDEXED,
	INDEXED_INDIRECT,
	INDIRECT_INDEXED,
} addressingMode;


mainMemory *powerUpMemory (void);
void powerDownMemory (mainMemory* mem);
u8* getPhysAddress (mainMemory *memory, u16 virtAddress);
