#include <stdio.h>
#include <6502.h>

void interruptRq (cpu6502 *cpu, interruptT irqType) {
  cpu->interruptV |= (1 << irqType);
}

/*Interrupt Priority : NMI (edge-triggered) > RESET (level-triggered) > IRQ (level-triggered)*/
void interruptSrvice (cpu6502 *cpu) {
  cpu->interruptV &= (~cpu->interruptV + 1); //eliminate LSB, i.e. service highest priority interrupt
}

void nmiHandler (cpu6502 *cpu) {
  stackPush8(cpu, (u8) (cpu->regPC >> 8));
  stackPush8(cpu, (u8) cpu->regPC);
  stackPush8(cpu, cpu->regP | 0x20);
  cpu->regPC = ((u16)*getPhysAddress(cpu->memory, 0xFFFA)) |
    (((u16)*getPhysAddress(cpu->memory, 0xFFFB)) << 8);
}

void resetHandler (cpu6502 *cpu) {
  cpu->regS -= 3;
  *getPhysAddress(cpu->memory, 0x4015) = 0x00;
  cpu->regPC = ((u16)*getPhysAddress(cpu->memory, 0xFFFC)) |
    (((u16)*getPhysAddress(cpu->memory, 0xFFFD)) << 8);
}

void irqHandler (cpu6502 *cpu, u8 bFlag) {
  stackPush8(cpu, (u8) (cpu->regPC >> 8));
  stackPush8(cpu, (u8) cpu->regPC);
  stackPush8(cpu, cpu->regP | bFlag);
  if (!statusFlagGet(cpu, I) || bFlag == 0x30)
    cpu->regPC = ((u16)*getPhysAddress(cpu->memory, 0xFFFE)) |
      (((u16)*getPhysAddress(cpu->memory, 0xFFFF)) << 8);
}

cpu6502 *powerUpCpu (void) {
  printf ("NES Version %f\n", (float) VERSION);
  cpu6502* cpu;

  cpu->regP = 0x34;
  cpu->regA = 0x00;
  cpu->regX = 0x00;
  cpu->regY = 0x00;
  cpu->regS = 0xFD;

  cpu->memory = powerUpMemory();

  cpu->indexRegAddrs[0] = (u64)(&cpu->regX);
  cpu->indexRegAddrs[1] = (u64)(&cpu->regY);
  cpu->indexRegAddrs[2] = (u64)(&cpu->regA);

  cpu->interruptV = 0;

  return cpu;
}

void powerDownCpu (cpu6502* cpu) {
  powerDownMemory (cpu->memory);
}

/*Lets refer to the guest's (NES) memory as "virtual"*/
void getVirtualAddress (cpu6502 *cpu, struct instruction *instr) {
	u8 os = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
	switch (instr->addrMode) {
		case ABSOLUTE_INDEXED: {
			instr->opData.addr += os;
			break;
		}
		case ZERO_PAGE_INDEXED: {
			instr->opData.addr = 0xFF & (instr->opData.addr + os);
			break;
		}
		case INDEXED_INDIRECT: {
			instr->opData.addr = ((u16)*getPhysAddress(cpu->memory, 0xFF & (instr->opData.addr + os))) |
				(((u16)*getPhysAddress(cpu->memory, 0xFF & (instr->opData.addr + os + 1))) << 8);
			break;
		}
		case INDIRECT_INDEXED: {
			instr->opData.addr = (((u16)*getPhysAddress(cpu->memory, instr->opData.addr)) |
				(((u16)*getPhysAddress(cpu->memory, instr->opData.addr + 1)) << 8)) + os;
			break;
		}
		default: {
			break;
		}
  }
}

bool statusFlagGet (cpu6502* cpu, flags flag) {
  return (bool)((1U << flag) & cpu->regP);
}

void statusFlagSet (cpu6502* cpu, flags flag, bool status) {
  cpu->regP = status ? (cpu->regP | (1 << flag)): (cpu->regP & ~(1 << flag));
}

u8 executeInstr (cpu6502* cpu) {
  u8 cyclesElapsed;

  if (cpu->interruptV != 0) {
    if ((bool) (cpu->interruptV & (1 << NMI)))
      nmiHandler(cpu);
    else if ((bool) (cpu->interruptV & (1 << RESET)))
      resetHandler(cpu);
    else if ((bool) (cpu->interruptV & (1 << IRQ)))
      irqHandler(cpu, 0x20);

    statusFlagSet(cpu, I, true);
    interruptSrvice(cpu);
    cyclesElapsed = 7;
  } else {
    u8 opcode = *getPhysAddress(cpu->memory, cpu->regPC);
    instruction instr = instructionMap[opcode];

    if (instr.addrMode == NON_MEMORY && instr.size >= 2)
      instr.opData.val = *getPhysAddress(cpu->memory, cpu->regPC + 1);
    else if (instr.size == 2)
      instr.opData.addr = (u16) *getPhysAddress(cpu->memory, cpu->regPC + 1);
    else if (instr.size == 3)
      instr.opData.addr = ((u16) *getPhysAddress(cpu->memory, cpu->regPC + 1)) |
        (((u16) *getPhysAddress(cpu->memory, cpu->regPC + 2)) << 8);

    cyclesElapsed = instr.exec(&instr, cpu);
    cpu->regPC += instr.size;
  }

  return cyclesElapsed;

}
