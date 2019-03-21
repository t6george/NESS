#include <instructions.h>

u8 readOperand (struct instruction *instr, cpu6502 *cpu) {
  u16 tgtAddr = instr->size == 2 ? instr->auxBytes[0]: ((u16)instr->auxBytes[1] << 8) | ((u16) instr->auxBytes[0]);
  u8 offset = (instr->srcReg == 0)? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
  return readByte (tgtAddr, instr->addrMode, cpu->memory, offset);
}
void writeBack (struct instruction *instr, cpu6502 *cpu, u8 byte) {
  u16 tgtAddr = instr->size == 2 ? instr->auxBytes[0]: ((u16)instr->auxBytes[1] << 8) | ((u16) instr->auxBytes[0]);
  u8 offset = (instr->srcReg == 0)? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1]);
  writeByte (byte, tgtAddr, instr->addrMode, cpu->memory, offset);
}

u8 BRK (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 ORA (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = (instr->addrMode == NON_MEMORY) ? instr->auxBytes[0]: readOperand (instr, cpu);
  cpu->regA |= operand;
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return instr->cycles;
}

u8 ASL (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = (instr->addrMode == NON_MEMORY) ? cpu->regA: readOperand (instr, cpu);
  statusFlagSet (cpu, C, (bool)(0x80 & operand));
  operand <<= 1;
  if (instr->addrMode == NON_MEMORY) {
    cpu->regA = operand;
  } else {
    writeBack (instr, cpu, operand);
  }
  statusFlagSet (cpu, N, (bool)(0x80 & operand));
  statusFlagSet (cpu, Z, operand == 0x00);
  return instr->cycles;
}

u8 BPL (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 CLC (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, C, false);
  return instr->cycles;
}

u8 JSR (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BIT (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 ROL (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 PLP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BMI (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 SEC (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, C, true);
  return instr->cycles;
}

u8 RTI (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 EOR (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = (instr->addrMode == NON_MEMORY) ? instr->auxBytes[0]: readOperand (instr, cpu);
  cpu->regA ^= operand;
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return instr->cycles;
}

u8 LSR (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = (instr->addrMode == NON_MEMORY) ? cpu->regA: readOperand (instr, cpu);
  statusFlagSet (cpu, C, (bool)(0x80 & operand));
  operand >>= 1;
  if (instr->addrMode == NON_MEMORY) {
    cpu->regA = operand;
  } else {
    writeBack (instr, cpu, operand);
  }
  statusFlagSet (cpu, N, (bool)(0x80 & operand));
  statusFlagSet (cpu, Z, operand == 0x00);
  return instr->cycles;
}

u8 PHA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 JMP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BVC (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
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
  return instr->cycles;
}

u8 PLA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BVS (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 SEI (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, I, true);
  return instr->cycles;
}

u8 STA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 STY (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 STX (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 DEY (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 TXA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BCC (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 TYA (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 TXS (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 TAY (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 TAX (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 LDA (struct instruction *instr, cpu6502 *cpu) {
  cpu->regA = readOperand (instr, cpu);
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return instr->cycles;
}

u8 LDX (struct instruction *instr, cpu6502 *cpu) {
  cpu->regX = readOperand (instr, cpu);
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return instr->cycles;
}

u8 LDY (struct instruction *instr, cpu6502 *cpu) {
  cpu->regY = readOperand (instr, cpu);
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return instr->cycles;
}

u8 BCS (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
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
  return instr->cycles;
}

u8 NOP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BEQ (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 SED (struct instruction *instr, cpu6502 *cpu) {
  statusFlagSet (cpu, D, true);
  return instr->cycles;
}

u8 DEX (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 AND (struct instruction *instr, cpu6502 *cpu) {
  u8 operand = (instr->addrMode == NON_MEMORY) ? instr->auxBytes[0]: readOperand (instr, cpu);
  cpu->regA &= operand;
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return instr->cycles;
}

u8 PHP (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 INY (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}

u8 BNE (struct instruction *instr, cpu6502 *cpu) {
  return instr->cycles;
}
