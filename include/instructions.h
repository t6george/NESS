#pragma once
#include <sysmem.h>
#include <6502.h>

typedef struct instruction {
  u8 size;
  addressingMode addrMode;
  u8 cycles;
  u8 (*exec)(struct instruction* instr, cpu6502 *cpu);
  regIndex srcReg;
  u8 auxBytes[2];
} instruction;

u8 BRK (struct instruction *instr, cpu6502 *cpu);
u8 ORA (struct instruction *instr, cpu6502 *cpu);
u8 ASL (struct instruction *instr, cpu6502 *cpu);
u8 BPL (struct instruction *instr, cpu6502 *cpu);
u8 CLC (struct instruction *instr, cpu6502 *cpu);
u8 JSR (struct instruction *instr, cpu6502 *cpu);
u8 BIT (struct instruction *instr, cpu6502 *cpu);
u8 ROL (struct instruction *instr, cpu6502 *cpu);
u8 PLP (struct instruction *instr, cpu6502 *cpu);
u8 BMI (struct instruction *instr, cpu6502 *cpu);
u8 SEC (struct instruction *instr, cpu6502 *cpu);
u8 RTI (struct instruction *instr, cpu6502 *cpu);
u8 EOR (struct instruction *instr, cpu6502 *cpu);
u8 LSR (struct instruction *instr, cpu6502 *cpu);
u8 PHA (struct instruction *instr, cpu6502 *cpu);
u8 JMP (struct instruction *instr, cpu6502 *cpu);
u8 BVC (struct instruction *instr, cpu6502 *cpu);
u8 CLI (struct instruction *instr, cpu6502 *cpu);
u8 RTS (struct instruction *instr, cpu6502 *cpu);
u8 ADC (struct instruction *instr, cpu6502 *cpu);
u8 ROR (struct instruction *instr, cpu6502 *cpu);
u8 PLA (struct instruction *instr, cpu6502 *cpu);
u8 BVS (struct instruction *instr, cpu6502 *cpu);
u8 SEI (struct instruction *instr, cpu6502 *cpu);
u8 STA (struct instruction *instr, cpu6502 *cpu);
u8 STY (struct instruction *instr, cpu6502 *cpu);
u8 STX (struct instruction *instr, cpu6502 *cpu);
u8 DEY (struct instruction *instr, cpu6502 *cpu);
u8 TXA (struct instruction *instr, cpu6502 *cpu);
u8 BCC (struct instruction *instr, cpu6502 *cpu);
u8 TYA (struct instruction *instr, cpu6502 *cpu);
u8 TXS (struct instruction *instr, cpu6502 *cpu);
u8 TAY (struct instruction *instr, cpu6502 *cpu);
u8 TAX (struct instruction *instr, cpu6502 *cpu);
u8 LDA (struct instruction *instr, cpu6502 *cpu);
u8 LDX (struct instruction *instr, cpu6502 *cpu);
u8 LDY (struct instruction *instr, cpu6502 *cpu);
u8 BCS (struct instruction *instr, cpu6502 *cpu);
u8 CLV (struct instruction *instr, cpu6502 *cpu);
u8 TSX (struct instruction *instr, cpu6502 *cpu);
u8 CPY (struct instruction *instr, cpu6502 *cpu);
u8 CMP (struct instruction *instr, cpu6502 *cpu);
u8 DEC (struct instruction *instr, cpu6502 *cpu);
u8 CLD (struct instruction *instr, cpu6502 *cpu);
u8 CPX (struct instruction *instr, cpu6502 *cpu);
u8 SBC (struct instruction *instr, cpu6502 *cpu);
u8 INC (struct instruction *instr, cpu6502 *cpu);
u8 INX (struct instruction *instr, cpu6502 *cpu);
u8 NOP (struct instruction *instr, cpu6502 *cpu);
u8 BEQ (struct instruction *instr, cpu6502 *cpu);
u8 SED (struct instruction *instr, cpu6502 *cpu);
u8 DEX (struct instruction *instr, cpu6502 *cpu);
u8 AND (struct instruction *instr, cpu6502 *cpu);
u8 PHP (struct instruction *instr, cpu6502 *cpu);
u8 INY (struct instruction *instr, cpu6502 *cpu);
u8 BNE (struct instruction *instr, cpu6502 *cpu);


static const instruction instructionMap [0x100] = {
  //0x0X
  {0, ABSOLUTE, 7, .exec = BRK},
  {1, INDEXED_INDIRECT, 6, .exec = ORA, X},
  {}, {}, {},
  {1, ZERO_PAGE, 3, .exec = ORA},
  {1, ZERO_PAGE, 5, .exec = ASL},
  {},
  {0, ABSOLUTE, 3, .exec = PHP},
  {1, NON_MEMORY, 2, .exec = ORA},
  {0, NON_MEMORY, 2, .exec = ASL, A},
  {}, {},
  {2, ABSOLUTE, 4, .exec = ORA},
  {2, ABSOLUTE, 6, .exec = ASL},
  {},
  //0x1X
  {1, NON_MEMORY, 2, .exec = BPL},
  {1, INDIRECT_INDEXED, 5, .exec = ORA, Y},
  {}, {}, {},
  {1, ZERO_PAGE_INDEXED, 4, .exec = ORA, X},
  {1, ZERO_PAGE_INDEXED, 6, .exec = ASL, X},
  {},
  {0, NON_MEMORY, 2, .exec = CLC},
  {2, ABSOLUTE_INDEXED, 4, .exec = ORA, Y},
  {}, {}, {},
  {2, ABSOLUTE_INDEXED, 4, .exec = ORA, X},
  {2, ABSOLUTE_INDEXED, 7, .exec = ASL, X},
  {},
  //0x2X
  {2, ABSOLUTE, 6, .exec = JSR},
  {1, INDEXED_INDIRECT, 6, .exec = AND, X},
  {},{},
  {1, ZERO_PAGE, 3, .exec = BIT},
  {1, ZERO_PAGE, 3, .exec = AND},
  {1, ZERO_PAGE, 5, .exec = ROL},
  {},
  {0, ABSOLUTE, 4, .exec = PLP},
  {1, NON_MEMORY, 2, .exec = AND},
  {0, NON_MEMORY, 2, .exec = ROL, A},
  {},
  {2, ABSOLUTE, 4, .exec = BIT},
  {2, ABSOLUTE, 4, .exec = AND},
  {2, ABSOLUTE, 6, .exec = ROL},
  {},
  //0x3X
  {1, NON_MEMORY, 2, .exec = BMI},
  {1, INDIRECT_INDEXED, 5, .exec = AND, Y},
  {},{},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = AND, X},
  {1, ZERO_PAGE_INDEXED, 6, .exec = ROL, X},
  {},
  {0, NON_MEMORY, 2, .exec = SEC},
  {2, ABSOLUTE_INDEXED, 4, .exec = AND, Y},
  {},{},{},
  {2, ABSOLUTE_INDEXED, 4, .exec = AND, X},
  {2, ABSOLUTE_INDEXED, 7, .exec = ROL, X},
  {},
  //0x4X
  {0, ABSOLUTE, 6, .exec = RTI},
  {1, INDEXED_INDIRECT, 6, .exec = EOR, X},
  {},{},{},
  {1, ZERO_PAGE, 3, .exec = EOR},
  {1, ZERO_PAGE, 5, .exec = LSR},
  {},
  {0, ABSOLUTE, 3, .exec = PHA},
  {0, NON_MEMORY, 2, .exec = EOR},
  {0, NON_MEMORY, 2, .exec = LSR, A},
  {},
  {2, ABSOLUTE, 3, .exec = JMP},
  {2, ABSOLUTE, 4, .exec = EOR},
  {2, ABSOLUTE, 6, .exec = LSR},
  {},
  //0x5X
  {1, NON_MEMORY, 2, .exec = BVC},
  {1, INDIRECT_INDEXED, 5, .exec = EOR, Y},
  {},{},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = EOR, X},
  {1, ZERO_PAGE_INDEXED, 6, .exec = LSR, X},
  {},
  {0, NON_MEMORY, 2, .exec = CLI},
  {2, ABSOLUTE_INDEXED, 4, .exec = EOR, Y},
  {},{},{},
  {2, ABSOLUTE_INDEXED, 4, .exec = EOR, X},
  {2, ABSOLUTE_INDEXED, 7, .exec = LSR, X},
  {},
  //0x6X
  {0, ABSOLUTE, 6, .exec = RTS},
  {1, INDEXED_INDIRECT, 6, .exec = ADC, X},
  {},{},{},
  {1, ZERO_PAGE, 3, .exec = ADC},
  {1, ZERO_PAGE, 5, .exec = ROR},
  {},
  {0, ABSOLUTE, 4, .exec = PLA},
  {1, NON_MEMORY, 2, .exec = ADC},
  {0, NON_MEMORY, 2, .exec = ROR, A},
  {},
  {2, INDIRECT_INDEXED, 5, .exec = JMP},
  {2, ABSOLUTE, 4, .exec = ADC},
  {2, ABSOLUTE, 6, .exec = ROR},
  {},
  //0x7X
  {1, NON_MEMORY, 2, .exec = BVS},
  {1, INDIRECT_INDEXED, 5, .exec = ADC, Y},
  {},{},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = ADC, X},
  {1, ZERO_PAGE_INDEXED, 6, .exec = ROR, X},
  {},
  {0, NON_MEMORY, 2, .exec = SEI},
  {2, ABSOLUTE_INDEXED, 4, .exec = ADC, Y},
  {},{},{},
  {2, ABSOLUTE_INDEXED, 4, .exec = ADC, X},
  {2, ABSOLUTE_INDEXED, 7, .exec = ROR, X},
  {},
  //0x8X
  {},
  {1, INDEXED_INDIRECT, 6, .exec = STA, X},
  {},{},
  {1, ZERO_PAGE, 3, .exec = STY},
  {1, ZERO_PAGE, 3, .exec = STA},
  {1, ZERO_PAGE, 3, .exec = STX},
  {},
  {0, NON_MEMORY, 2, .exec = DEY},
  {},
  {0, NON_MEMORY, 2, .exec = TXA},
  {},
  {2, ABSOLUTE, 4, .exec = STY},
  {2, ABSOLUTE, 4, .exec = STA},
  {2, ABSOLUTE, 4, .exec = STX},
  {},
  //0x9X
  {1, NON_MEMORY, 2, .exec = BCC},
  {1, INDIRECT_INDEXED, 6, .exec = STA, Y},
  {},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = STY, X},
  {1, ZERO_PAGE_INDEXED, 4, .exec = STA, X},
  {1, ZERO_PAGE_INDEXED, 4, .exec = STX, Y},
  {},
  {0, NON_MEMORY, 2, .exec = TYA},
  {2, ABSOLUTE, 5, .exec = STA, Y},
  {0, NON_MEMORY, 2, .exec = TXS},
  {},{},
  {2, ABSOLUTE, 5, .exec = STA, X},
  {},{},
  //0xAX
  {1, NON_MEMORY, 2, .exec = LDY},
  {1, INDEXED_INDIRECT, 6, .exec = LDA, X},
  {1, NON_MEMORY, 2, .exec = LDX},
  {},
  {1, ZERO_PAGE, 3, .exec = LDY},
  {1, ZERO_PAGE, 3, .exec = LDA},
  {1, ZERO_PAGE, 3, .exec = LDX},
  {},
  {0, NON_MEMORY, 2, .exec = TAY},
  {1, NON_MEMORY, 2, .exec = LDA},
  {0, NON_MEMORY, 2, .exec = TAX},
  {},
  {2, ABSOLUTE, 4, .exec = LDY},
  {2, ABSOLUTE, 4, .exec = LDA},
  {2, ABSOLUTE, 4, .exec = LDX},
  {},
  //0xBX
  {1, NON_MEMORY, 2, .exec = BCS},
  {1, INDIRECT_INDEXED, 5, .exec = LDA, Y},
  {},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = LDY, X},
  {1, ZERO_PAGE_INDEXED, 4, .exec = LDA, X},
  {1, ZERO_PAGE_INDEXED, 4, .exec = LDX, Y},
  {},
  {0, NON_MEMORY, 2, .exec = CLV},
  {1, INDIRECT_INDEXED, 5, .exec = LDA, Y},
  {0, NON_MEMORY, 2, .exec = TSX},
  {},
  {2, ABSOLUTE_INDEXED, 4, .exec = LDY, X},
  {2, ABSOLUTE_INDEXED, 4, .exec = LDA, X},
  {2, ABSOLUTE_INDEXED, 4, .exec = LDX, Y},
  {},
  //0xCX
  {1, ABSOLUTE, 2, .exec = CPY},
  {1, INDEXED_INDIRECT, 6, .exec = CMP, X},
  {},{},
  {1, ZERO_PAGE, 3, .exec = CPY},
  {1, ZERO_PAGE, 3, .exec = CMP},
  {1, ZERO_PAGE, 5, .exec = DEC},
  {},
  {0, NON_MEMORY, 2, .exec = INY},
  {1, NON_MEMORY, 2, .exec = CMP},
  {0, NON_MEMORY, 2, .exec = DEX},
  {},
  {2, ABSOLUTE, 4, .exec = CPY},
  {2, ABSOLUTE, 4, .exec = CMP},
  {2, ABSOLUTE, 3, .exec = DEC},
  {},
  //0xDX
  {1, NON_MEMORY, 2, .exec = BNE},
  {1, INDIRECT_INDEXED, 5, .exec = CMP, Y},
  {},{},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = CMP, X},
  {1, ZERO_PAGE_INDEXED, 6, .exec = DEC, X},
  {},
  {0, NON_MEMORY, 2, .exec = CLD},
  {2, ABSOLUTE_INDEXED, 4, .exec = CMP, Y},
  {},{},{},
  {2, ABSOLUTE, 4, .exec = CMP, X},
  {2, ABSOLUTE, 3, .exec = DEC, X},
  {},
  //0xEX
  {1, NON_MEMORY, 2, .exec = CPX},
  {1, INDEXED_INDIRECT, 6, .exec = SBC, X},
  {},{},
  {1, ZERO_PAGE, 3, .exec = CPX},
  {1, ZERO_PAGE, 3, .exec = SBC},
  {1, ZERO_PAGE, 5, .exec = INC},
  {},
  {0, NON_MEMORY, 2, .exec = INX},
  {1, NON_MEMORY, 2, .exec = SBC},
  {0, NON_MEMORY, 2, .exec = NOP},
  {},
  {2, ABSOLUTE, 4, .exec = CPX},
  {2, ABSOLUTE, 4, .exec = SBC},
  {2, ABSOLUTE, 6, .exec = INC},
  {},
  //0xFX
  {1, NON_MEMORY, 2, .exec = BEQ},
  {1, INDIRECT_INDEXED, 5, .exec = SBC, Y},
  {},{},{},
  {1, ZERO_PAGE_INDEXED, 4, .exec = SBC, X},
  {1, ZERO_PAGE_INDEXED, 6, .exec = INC, X},
  {},
  {0, NON_MEMORY, 2, .exec = SED},
  {2, ABSOLUTE_INDEXED, 4, .exec = SBC, Y},
  {},{},{},
  {2, ABSOLUTE_INDEXED, 4, .exec = SBC, X},
  {2, ABSOLUTE_INDEXED, 7, .exec = INC, X},
  {},
};
