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
	u64* memory [MEM_MAP_COUNT];
	u8 ram [RAM_RANGE];
	u8 ppuRegs [PPU_REGS_RANGE];
	u8 apuRegs [APU_REGS_RANGE];
	u8 cartridgeMem [CART_SPACE_RANGE];
} mainMemory;


u8 initMemory (void);
u64* decodeAddress (addr address);
u16 readMem (addr address, u8 addrMode);
u8 writeMem (u8 data, addr address, u8 addrMode);

