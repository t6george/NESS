#include "6502.h"

#define RAM_RANGE 0x800
#define PPU_REGS_RANGE 0x8
#define APU_REGS_RANGE 0x18
#define CART_SPACE_RANGE 0xBFE0
#define MEM_MAP_COUNT 0x4

typedef union {
	u8 singleByte[1];
	u8 doubleByte[2];
} addr;

typedef struct {
	u8* memory [MEM_MAP_COUNT];
	u8 ram [RAM_RANGE];
	u8 ppuRegs [PPU_REGS_RANGE];
	u8 apuRegs [APU_REGS_RANGE];
	u8 cartridgeMem [CART_SPACE_RANGE];
} mainMemory;


mainMemory *initMemory (void);
u8** decodeAddress (addr address);
u8 readByte (addr address, u8 addrMode);
void writeByte (u8 data, addr address, u8 addrMode);

