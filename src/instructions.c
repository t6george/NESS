#include "instructions.h"
#include <assert.h>

// u16 address, addressingMode addrMode, mainMemory *memory, u8 os

u8 BRK (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 ORA (instruction *instr, cpu6502 *cpu) {
  u8 operand;
  if (instr->addrMode == NON_MEMORY) {
    operand = instr->auxBytes[0];
  } else {
    u16 tgtAddr = instr->size == 2 ? instr->auxBytes[0]: ((u16)instr->auxBytes[0] << 8) | ((u16) instr->auxBytes[1]);
    u8 offset = (instr->srcReg == 0)? 0: *((u8*)cpu->indexRegAddrs[instr->srcReg-1])
    operand = readByte (tgtAddr, instr->addrMode, cpu->memory, offset);
  }
  cpu->regA |= operand;
  statusFlagSet (cpu, Z, cpu->regA == 0x0);
  statusFlagSet (cpu, N, (0x80 & cpu->regA) != 0x0);
  return 0;
}

u8 ASL (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BPL (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CLC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 JSR (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BIT (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 ROL (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 PLP (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BMI (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 PLP (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 SEC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 RTI (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 EOR (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 LSR (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 PHA (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 JMP (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BVC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CLI (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 RTS (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 ADC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 ROR (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 PLA (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BVS (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 SEI (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 STA (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 STY (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 STX (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 DEY (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 TXA (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BCC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 TYA (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 TXS (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 TAY (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 TAX (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 LDA (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 LDX (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 LDY (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BCS (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CLV (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 TSX (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CPY (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CMP (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 DEC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CLD (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 CPX (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 SBC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 INC (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 INX (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 NOP (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 BEQ (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 SED (instruction *instr, cpu6502 *cpu) {
  return 0;
}

u8 DEX (instruction *instr, cpu6502 *cpu) {
  return 0;
}
