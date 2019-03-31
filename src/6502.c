#include <stdio.h>

#include <6502.h>

static mainMemory *memory;

cpu6502* powerUpCpu (void) {
  printf ("NES Version %f\n", (float) VERSION);
  cpu6502* cpu;

  cpu->regP = 0x34;
  cpu->regA = 0x00;
  cpu->regX = 0x00;
  cpu->regY = 0x00;
  cpu->regS = 0xFD;

  cpu->memory = powerUpMemory ();

  cpu->indexRegAddrs[0] = (u64)(&cpu->regX);
  cpu->indexRegAddrs[1] = (u64)(&cpu->regY);
  cpu->indexRegAddrs[2] = (u64)(&cpu->regA);
  return cpu;
}

void powerDownCpu (cpu6502* cpu) {
  powerDownMemory (cpu->memory);
}

void resetCpu (cpu6502* cpu) {
  cpu->regS -= 0x03;
  cpu->regP |= 0x04;
  writeByte (0x00, 0x1540, ABSOLUTE, cpu->memory, 0x00);
}

bool statusFlagGet (cpu6502* cpu, flags flag) {
  return (bool)((1U << flag) & cpu->regP);
}

void statusFlagSet (cpu6502* cpu, flags flag, bool status) {
  cpu->regP = status ? (cpu->regP | (1U << flag)): (cpu->regP & ~(1U << flag));
}

u8 stepInstr (cpu6502* cpu) {
  // u8 opcode = cpu->memory (cpu->regPC++, u8 addrMode, cpu->mainMemory);
  if (instr->size == 2)
    address = (u16) instr->auxBytes[0];
  else
    address = ((u16)instr->auxBytes[1] << 8) | ((u16) instr->auxBytes[0]);
  for (u8 b = 0; b < 1; b++) {
    break;
  }
  return 0;
}
