#include <instructions.h>

#define IS_PAGE_CROSSED(a1, a2) (0xFF & a1) != (0xFF & a2)

#define READ_IN(instr, cpu, dst) ({
  u16 tgtAddr = instr->size == 2 ? instr->auxBytes[0]:
    ((u16)instr->auxBytes[1] << 8) | ((u16) instr->auxBytes[0]);\
  u8 offset = (instr->srcReg == 0)? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);\
  dst = readByte (tgtAddr, instr->addrMode, cpu->memory, offset);
})

#define WRITE_BACK(instr, cpu, byte) ({
  u16 tgtAddr = instr->size == 2 ? instr->auxBytes[0]:
    ((u16)instr->auxBytes[1] << 8) | ((u16) instr->auxBytes[0]);\
  u8 offset = (instr->srcReg == 0)? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);\
  writeByte (byte, tgtAddr, instr->addrMode, cpu->memory, offset);
})

#define SET_NZ (cpu, op) ({
  statusFlagSet (cpu, Z, op == 0x0);\
  statusFlagSet (cpu, N, (0x80 & op) != 0x0);
})

u8 BRK (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 ORA (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  if (instr->addrMode == NON_MEMORY)
    operand = cpu->regA;
  else
    READ_IN (instr, cpu, operand)
  cpu->regA |= operand;
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 ASL (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  if (instr->addrMode == NON_MEMORY)
    operand = cpu->regA;
  else
    READ_IN (instr, cpu, operand)
  statusFlagSet (cpu, C, (0x80 & operand) != 0x0);
  operand <<= 1;
  if (instr->addrMode == NON_MEMORY)
    cpu->regA = operand;
  else
    WRITE_BACK (instr, cpu, operand)
  SET_NZ (cpu, operand)
  return instr->cycles;
}

u8 BPL (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, N);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 CLC (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, C, false);
  return instr->cycles;
}

u8 JSR (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BIT (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  READ_IN (instr, cpu, operand)
  statusFlagSet (cpu, Z, (cpu->regA & operand) == 0);
  statusFlagSet (cpu, V, (0x80 & operand) != 0x0);
  statusFlagSet (cpu, N, (0x40 & operand) != 0x0);
  return instr->cycles;
}

u8 ROL (struct instruction *instr, cpu6502 *cpu) {
  u8 oldCarry = (u8) statusFlagGet (cpu, C);
  u8 operand;
  if (instr->addrMode == NON_MEMORY)
    operand = cpu->regA;
  else
    READ_IN (instr, cpu, operand)
  statusFlagSet (cpu, C, (0x80 & operand) != 0x0);
  operand = (operand << 1) | oldCarry;
  if (instr->addrMode == NON_MEMORY)
    cpu->regA = operand;
  else
    WRITE_BACK (instr, cpu, operand)
  SET_NZ (cpu, operand)
  return instr->cycles;
}

u8 PLP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BMI (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, N);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 SEC (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, C, true);
  return instr->cycles;
}

u8 RTI (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 EOR (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  if (instr->addrMode == NON_MEMORY)
    operand = instr->auxBytes[0];
  else
    READ_IN (instr, cpu, operand)
  cpu->regA ^= operand;
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 LSR (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  if (instr->addrMode == NON_MEMORY)
    operand = cpu->regA;
  else
    READ_IN (instr, cpu, operand)
  statusFlagSet (cpu, C, (0x80 & operand) != 0x0);
  operand >>= 1;
  if (instr->addrMode == NON_MEMORY)
    cpu->regA = operand;
  else
    WRITE_BACK (instr, cpu, operand)
  SET_NZ (cpu, operand)
  return instr->cycles;
}

u8 PHA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 JMP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BVC (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, V);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 CLI (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, I, false);
  return instr->cycles;
}

u8 RTS (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 ADC (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 ROR (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  u8 oldCarry = ((u8) statusFlagGet (cpu, C)) << 0x7;
  if (instr->addrMode == NON_MEMORY) {
    operand = cpu->regA;
  } else {
    READ_IN (instr, cpu, operand)
  }
  statusFlagSet (cpu, C, (0x01 & operand) == 0x01);
  operand = (operand >> 1) | oldCarry;
  if (instr->addrMode == NON_MEMORY) {
    cpu->regA = operand;
  } else {
    WRITE_BACK (instr, cpu, operand)
  }
  SET_NZ (cpu, operand)
  return instr->cycles;
}

u8 PLA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BVS (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, V);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 SEI (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, I, true);
  return instr->cycles;
}

u8 STA (struct instruction *instr, cpu6502 *cpu) {
  WRITE_BACK (instr, cpu, cpu->regA)
  return instr->cycles;
}

u8 STY (struct instruction *instr, cpu6502 *cpu) {
  WRITE_BACK (instr, cpu, cpu->regY)
  return instr->cycles;
}

u8 STX (struct instruction *instr, cpu6502 *cpu) {
  WRITE_BACK (instr, cpu, cpu->regX)
  return instr->cycles;
}

u8 DEY (struct instruction *instr, cpu6502 *cpu) {
  --cpu->regY;
  SET_NZ (cpu, cpu->regY)
  return instr->cycles;
}

u8 TXA (struct instruction *instr, cpu6502 *cpu) {
  cpu->regA = cpu->regX;
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 BCC (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, C);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 TYA (struct instruction *instr, cpu6502 *cpu) {
  cpu->regA = cpu->regY;
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 TXS (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 TAY (struct instruction *instr, cpu6502 *cpu) {
  cpu->regY = cpu->regA;
  SET_NZ (cpu, cpu->regY)
  return instr->cycles;
}

u8 TAX (struct instruction *instr, cpu6502 *cpu) {
  cpu->regX = cpu->regA;
  SET_NZ (cpu, cpu->regX)
  return instr->cycles;
}

u8 LDA (struct instruction *instr, cpu6502 *cpu) {
  READ_IN (instr, cpu, (cpu->regA))
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 LDX (struct instruction *instr, cpu6502 *cpu) {
  READ_IN (instr, cpu, (cpu->regX))
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 LDY (struct instruction *instr, cpu6502 *cpu) {
  READ_IN (instr, cpu, (cpu->regY))
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 BCS (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, C);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 CLV (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, V, false);
  return instr->cycles;
}

u8 TSX (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 CPY (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 CMP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 DEC (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 CLD (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, D, false);
  return instr->cycles;
}

u8 CPX (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 SBC (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 INC (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 INX (struct instruction *instr, cpu6502 *cpu) {
  ++cpu->regX;
  SET_NZ (cpu, cpu->regX)
  return instr->cycles;
}

u8 NOP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BEQ (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = statusFlagGet (cpu, Z);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}

u8 SED (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, D, true);
  return instr->cycles;
}

u8 DEX (struct instruction *instr, cpu6502 *cpu) {
  --cpu->regX;
  SET_NZ (cpu, cpu->regX)
  return instr->cycles;
}

u8 AND (struct instruction *instr, cpu6502 *cpu) {
  u8 operand;
  if (instr->addrMode == NON_MEMORY)
    operand = instr->auxBytes[0];
  else
    READ_IN (instr, cpu, operand)
  cpu->regA &= operand;
  SET_NZ (cpu, cpu->regA)
  return instr->cycles;
}

u8 PHP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 INY (struct instruction *instr, cpu6502 *cpu) {
  ++cpu->regY;
  SET_NZ (cpu, cpu->regY)
  return instr->cycles;
}

u8 BNE (struct instruction *instr, cpu6502 *cpu) {
  bool toBranch = !statusFlagGet (cpu, Z);
  i8 offset = 0;
  if (toBranch) {
    offset = (i8) intr->auxBytes[0]
    cpu->regPC += offset; }
  return instr->cycles + (u8)toBranch +
    (u8)(IS_PAGE_CROSSED((cpu->regPC - offset), cpu->regPC));
}
