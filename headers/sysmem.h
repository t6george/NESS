#include "6502.h"

#define RAM_RANGE 0x800
#define PPU_REGS_RANGE 0x8
#define APU_IO_REGS_RANGE 0x8
#define APU_REGS_RANGE 0x18
#define CART_SPACE_RANGE 0xBFE0

typedef struct {
	u8 ram [RAM_RANGE];
	u8 ppuRegs [PPU_REGS_RANGE];
	u8 apuRegs [APU_REGS_RANGE];
	u8 apuIORegs [APU_IO_REGS_RANGE];
	u8 cartridgeMem [CART_SPACE_RANGE];
} mainMemory;


mainMemory *initMemory (void);
u8* decodeAddress (u16 address, mainMemory *memory);
u8 readByte (u16 address, u8 addrMode, mainMemory *memory);
void writeByte (u8 data, u16 address, u8 addrMode, mainMemory *memory);
