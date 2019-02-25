#include "6502.h"

#define RAM_RANGE 0x800
#define PPU_REGS_RANGE 0x8
#define APU_IO_REGS_RANGE 0x8
#define APU_REGS_RANGE 0x18
#define CART_SPACE_RANGE 0xBFE0

typedef struct {
	u8 *ram = NULL;
	u8 *ppuRegs = NULL;
	u8 *apuRegs = NULL;
	u8 *apuIORegs = NULL;
	u8 *cartridgeMem = NULL;

} mainMemory;

enum addressingMode {
	ABSOLUTE = 0,
	ZERO_PAGE,
	ABSOLUTE_INDEXED,
	ZERO_PAGE_INDEXED,
	INDEXED_INDIRECT,
	INDIRECT_INDEXED,
};


mainMemory *powerUpMemory (void);
void powerDownMemory (mainMemory* mem);
u8* decodeAddress (u16 address, mainMemory *memory);
u8 readByte (u16 address, u8 addrMode, mainMemory *memory, u8 os);
void writeByte (u8 data, u16 address, u8 addrMode, mainMemory *memory, u8 os);
