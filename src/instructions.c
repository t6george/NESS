#include "instructions.h"
#include <assert.h>

// u16 address, addressingMode addrMode, mainMemory *memory, u8 os

u8 orWithAccumulator (cpu6502 *cpu, instruction *instr) {
  if (instr->size == 1) {
    cpu->regA |= readByte (intr->(*auxBytes), instr->addrMode, cpu->memory);
  } else {
    cpu->regA |= readByte ();
  }

  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
}
