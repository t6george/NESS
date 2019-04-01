#include <stdio.h>
#include <6502.h>

cpu6502 *powerUpCpu (void) {
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
  *getPhysAddress(cpu->memory, 0x1540) = 0x00;
}

/*Lets refer to the guest's (NES) memory as "virtual"*/
void getVirtualAddress (cpu6502 *cpu, struct instruction *instr) {
	u8 os = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
	u16 virtAddress = instr->size == 3 ? instr->opData.addr: (instr->opData.addr >> 8);

	switch (instr->addrMode) {
		case ABSOLUTE_INDEXED: {
			virtAddress += os;
			break;
		}
		case ZERO_PAGE_INDEXED: {
			virtAddress = 0xFF & (virtAddress + os);
			break;
		}
		case INDEXED_INDIRECT: {
			virtAddress = ((u16)*getPhysAddress(cpu->memory, 0xFF & (virtAddress + os))) |
				(((u16)*getPhysAddress(cpu->memory, 0xFF & (virtAddress + os + 1))) << 8);
			break;
		}
		case INDIRECT_INDEXED: {
			virtAddress = (((u16)*getPhysAddress(cpu->memory, virtAddress)) |
				(((u16)*getPhysAddress(cpu->memory, virtAddress + 1)) << 8)) + os;
			break;
		}
		default: {
			break;
		}
		instr->opData.addr = virtAddress;
  }
}

bool statusFlagGet (cpu6502* cpu, flags flag) {
  return (bool)((1U << flag) & cpu->regP);
}

void statusFlagSet (cpu6502* cpu, flags flag, bool status) {
  cpu->regP = status ? (cpu->regP | (1U << flag)): (cpu->regP & ~(1U << flag));
}

u8 stepInstr (cpu6502* cpu) {
  // u8 opcode = cpu->memory (cpu->regPC++, u8 addrMode, cpu->mainMemory);
  for (u8 b = 0; b < 1; b++) {
    break;
  }
  return 0;
}
