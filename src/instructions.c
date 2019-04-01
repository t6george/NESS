#include <instructions.h>

#define PAGE_PENALTY(a1, a2) (u8) (0xFF00 & (a1)) != (0xFF00 & (a2)) *\
            (u8) (instr->addrMode == ABSOLUTE_INDEXED || instr->addrMode == INDIRECT_INDEXED)

#define SET_NZ(op) do {\
            statusFlagSet (cpu, Z, op == 0x0);\
            statusFlagSet (cpu, N, (0x80 & op) != 0x0);\
          } while(0);

u8 stackPull8(cpu6502* cpu) {
  return *getPhysAddress(cpu->memory, ++cpu->regS + 0x100);
}

void stackPush8(cpu6502* cpu, u8 byte) {
  *getPhysAddress(cpu->memory, 0x100 + cpu->regS--) = byte;
}

u8 BRK (struct instruction *instr, cpu6502 *cpu) {

  return instr->cycles;
}

u8 ORA (struct instruction *instr, cpu6502 *cpu) {
  u8 operand, pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    operand = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  cpu->regA |= operand;
  SET_NZ(cpu->regA)
  return instr->cycles + pagePenalty;
}

u8 ASL (struct instruction *instr, cpu6502 *cpu) {
  u8* operand;
  if (instr->addrMode == NON_MEMORY)
    operand = &cpu->regA;
  else
    operand = getPhysAddress(cpu->memory, instr->opData.addr);
  statusFlagSet (cpu, C, (0x80 & *operand) != 0x0);
  *operand <<= 1;
  SET_NZ(*operand)
  return instr->cycles;
}

u8 BPL (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet(cpu, N); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC));
}

u8 CLC (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, C, false);
  return instr->cycles;
}

u8 JSR (struct instruction *instr, cpu6502 *cpu) {
  stackPush8(cpu, (u8) ((cpu->regPC - 1) >> 8));
  stackPush8(cpu, (u8) (cpu->regPC - 1));
  cpu->regPC = instr->opData.addr;
  return instr->cycles;
}

u8 BIT (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = *getPhysAddress(cpu->memory, instr->opData.addr);
  statusFlagSet (cpu, Z, (cpu->regA & operand) == 0);
  statusFlagSet (cpu, V, (0x80 & operand) != 0x0);
  statusFlagSet (cpu, N, (0x40 & operand) != 0x0);
  return instr->cycles;
}

u8 ROL (struct instruction *instr, cpu6502 *cpu) {
  u8 oldCarry = (u8) statusFlagGet (cpu, C); u8* operand;
  if (instr->addrMode == NON_MEMORY)
    operand = &cpu->regA;
  else
    operand = getPhysAddress(cpu->memory, instr->opData.addr);
  statusFlagSet (cpu, C, (0x80 & *operand) != 0x0);
  *operand = (*operand << 1) | oldCarry;
  SET_NZ(*operand)
  return instr->cycles;
}

u8 PLP (struct instruction *instr, cpu6502 *cpu) {
  cpu->regP = stackPull8(cpu);
  SET_NZ(cpu->regP)
  return instr->cycles;
}

u8 BMI (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, N); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC));
}

u8 SEC (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, C, true);
  return instr->cycles;
}

u8 RTI (struct instruction *instr, cpu6502 *cpu) {
  cpu->regS = stackPull8(cpu);
  u16 execAddr = (u16) stackPull8(cpu);
  execAddr = (execAddr | (((u16) stackPull8(cpu)) << 8)) + 1;
  cpu->regPC = execAddr;
  return instr->cycles;
}

u8 EOR (struct instruction *instr, cpu6502 *cpu) {
  u8 operand, pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    operand = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  cpu->regA ^= operand;
  SET_NZ(cpu->regA)
  return instr->cycles + pagePenalty;
}

u8 LSR (struct instruction *instr, cpu6502 *cpu) {
  u8 *operand;
  if (instr->addrMode == NON_MEMORY)
    operand = &cpu->regA;
  else
    operand = getPhysAddress(cpu->memory, instr->opData.addr);
  statusFlagSet (cpu, C, (0x80 & *operand) != 0x0);
  *operand >>= 1;
  SET_NZ(*operand)
  return instr->cycles;
}

u8 PHA (struct instruction *instr, cpu6502 *cpu) {
  stackPush8(cpu, cpu->regA);
  return instr->cycles;
}

u8 JMP (struct instruction *instr, cpu6502 *cpu) {
  cpu->regPC = *getPhysAddress(cpu->memory, instr->opData.addr);
  return instr->cycles;
}

u8 BVC (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, V); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC));
}

u8 CLI (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, I, false);
  return instr->cycles;
}

u8 RTS (struct instruction *instr, cpu6502 *cpu) {
  u16 execAddr = (u16) stackPull8(cpu);
  execAddr = (execAddr | (((u16) stackPull8(cpu)) << 8)) + 1;
  cpu->regPC = execAddr;
  return instr->cycles;
}

u8 ADC (struct instruction *instr, cpu6502 *cpu) {
  u8 operand, pagePenalty = 0; u16 sum;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    operand = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }

  sum = cpu->regA + operand + (u8) statusFlagGet (cpu, C);
  statusFlagSet (cpu, C, 0xFF < sum);
  statusFlagSet (cpu, V, ((operand ^ sum) & (cpu->regA ^ sum) & 0x80) != 0);
  cpu->regA = (u8) sum;
  SET_NZ (cpu->regA)
  return instr->cycles + pagePenalty;
}

u8 ROR (struct instruction *instr, cpu6502 *cpu) {
  u8 oldCarry = ((u8) statusFlagGet (cpu, C)) << 0x7; u8 *operand;

  if (instr->addrMode == NON_MEMORY)
    operand = &cpu->regA;
  else
    operand = getPhysAddress(cpu->memory, instr->opData.addr);

  statusFlagSet (cpu, C, (0x01 & *operand) == 0x01);
  *operand = (*operand >> 1) | oldCarry;
  SET_NZ(*operand)
  return instr->cycles;
}

u8 PLA (struct instruction *instr, cpu6502 *cpu) {
  cpu->regA = stackPull8(cpu);
  SET_NZ(cpu->regA)
  return instr->cycles;
}

u8 BVS (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, V); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC));
}

u8 SEI (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, I, true);
  return instr->cycles;
}

u8 STA (struct instruction *instr, cpu6502 *cpu) {
  *getPhysAddress(cpu->memory, instr->opData.addr) = cpu->regA;
  return instr->cycles;
}

u8 STY (struct instruction *instr, cpu6502 *cpu) {
  *getPhysAddress(cpu->memory, instr->opData.addr) = cpu->regY;
  return instr->cycles;
}

u8 STX (struct instruction *instr, cpu6502 *cpu) {
  *getPhysAddress(cpu->memory, instr->opData.addr) = cpu->regX;
  return instr->cycles;
}

u8 DEY (struct instruction *instr, cpu6502 *cpu) {
  --cpu->regY;
  SET_NZ(cpu->regY)
  return instr->cycles;
}

u8 TXA (struct instruction *instr, cpu6502 *cpu) {
  cpu->regA = cpu->regX;
  SET_NZ(cpu->regA)
  return instr->cycles;
}

u8 BCC (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, C); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC));
}

u8 TYA (struct instruction *instr, cpu6502 *cpu) {
  cpu->regA = cpu->regY;
  SET_NZ(cpu->regA)
  return instr->cycles;
}

u8 TXS (struct instruction *instr, cpu6502 *cpu) {
  cpu->regS = cpu->regX;
  return instr->cycles;
}

u8 TAY (struct instruction *instr, cpu6502 *cpu) {
  cpu->regY = cpu->regA;
  SET_NZ(cpu->regY)
  return instr->cycles;
}

u8 TAX (struct instruction *instr, cpu6502 *cpu) {
  cpu->regX = cpu->regA;
  SET_NZ(cpu->regX)
  return instr->cycles;
}

u8 LDA (struct instruction *instr, cpu6502 *cpu) {
  u8 pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    cpu->regA = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    cpu->regA = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  SET_NZ(cpu->regA)
  return instr->cycles + pagePenalty;
}

u8 LDX (struct instruction *instr, cpu6502 *cpu) {
  u8 pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    cpu->regX = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    cpu->regX = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  SET_NZ(cpu->regX)
  return instr->cycles + pagePenalty;
}

u8 LDY (struct instruction *instr, cpu6502 *cpu) {
  u8 pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    cpu->regY = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    cpu->regY = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  SET_NZ(cpu->regY)
  return instr->cycles + pagePenalty;
}

u8 BCS (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, C); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC));
}

u8 CLV (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, V, false);
  return instr->cycles;
}

u8 TSX (struct instruction *instr, cpu6502 *cpu) {
  cpu->regX = cpu->regS;
  return instr->cycles;
}

u8 CPY (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = *getPhysAddress(cpu->memory, instr->opData.addr);
  statusFlagSet (cpu, C, cpu->regY >= operand);
  SET_NZ((cpu->regY - operand))
  return instr->cycles;
}

u8 CMP (struct instruction *instr, cpu6502 *cpu) {
  u8 operand, pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    operand = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  statusFlagSet (cpu, C, cpu->regA >= operand);
  SET_NZ((cpu->regA - operand))
  return instr->cycles + pagePenalty;
}

u8 DEC (struct instruction *instr, cpu6502 *cpu) {
  u8 *operand = getPhysAddress(cpu->memory, instr->opData.addr);
  --(*operand);
  SET_NZ(*operand)
  return instr->cycles;
}

u8 CLD (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, D, false);
  return instr->cycles;
}

u8 CPX (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = *getPhysAddress(cpu->memory, instr->opData.addr);
  statusFlagSet (cpu, C, cpu->regX >= operand);
  SET_NZ((cpu->regX - operand))
  return instr->cycles;
}

u8 SBC (struct instruction *instr, cpu6502 *cpu) {
  u8 operand, pagePenalty = 0; u16 diff;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    operand = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }

  operand = ~operand;
  diff = cpu->regA + operand + (u8) statusFlagGet (cpu, C);
  statusFlagSet (cpu, C, 0xFF < diff);
  statusFlagSet (cpu, V, ((operand ^ diff) & (cpu->regA ^ diff) & 0x80) != 0);
  cpu->regA = (u8) diff;
  SET_NZ (cpu->regA)
  return instr->cycles + pagePenalty;
}

u8 INC (struct instruction *instr, cpu6502 *cpu) {
  u8 *operand = getPhysAddress(cpu->memory, instr->opData.addr);
  ++(*operand);
  SET_NZ(*operand)
  return instr->cycles;
}

u8 INX (struct instruction *instr, cpu6502 *cpu) {
  ++cpu->regX;
  SET_NZ(cpu->regX)
  return instr->cycles;
}

u8 NOP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BEQ (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, Z); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC);
}

u8 SED (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet(cpu, D, true);
  return instr->cycles;
}

u8 DEX (struct instruction *instr, cpu6502 *cpu) {
  --cpu->regX;
  SET_NZ(cpu->regX)
  return instr->cycles;
}

u8 AND (struct instruction *instr, cpu6502 *cpu) {
  u8 operand, pagePenalty = 0;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->opData.val;
  } else {
    u8 regOs = instr->srcReg == 0 ? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
    operand = *getPhysAddress(cpu->memory, instr->opData.addr);
    pagePenalty = PAGE_PENALTY((instr->opData.addr - regOs), instr->opData.addr); }
  cpu->regA &= operand;
  SET_NZ(cpu->regA)
  return instr->cycles + pagePenalty;
}

u8 PHP (struct instruction *instr, cpu6502 *cpu) {
  stackPush8(cpu, cpu->regP);
  return instr->cycles;
}

u8 INY (struct instruction *instr, cpu6502 *cpu) {
  ++cpu->regY;
  SET_NZ(cpu->regY)
  return instr->cycles;
}

u8 BNE (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, Z); i8 offset = 0;
  if (toBranch) {
    offset = (i8) instr->opData.val;
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    PAGE_PENALTY((cpu->regPC - offset + instr->size), cpu->regPC);
}
