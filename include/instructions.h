#pragma once
#include <sysmem.h>
#include <6502.h>

typedef struct instruction {
  u8 size;
  addressingMode addrMode;
  u8 cycles;
  u8 (*exec)(struct instruction* instr, cpu6502 *cpu);
  regIndex srcReg;
  union {
    u8 val;
    u16 addr;
  } opData;
} instruction;

u8 stackPull8(cpu6502* cpu);
void stackPush8(cpu6502* cpu, u8 byte);
u16 constructAddress (instruction *instr);
u8 sysRead (cpu6502 *cpu, instruction *instr, u16 address);
void sysWrite (cpu6502 *cpu, instruction *instr, u16 address, u8 byte);

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
  {1, ABSOLUTE, 7, .exec = BRK},
  {2, INDEXED_INDIRECT, 6, .exec = ORA, X},
  {}, {}, {},
  {2, ZERO_PAGE, 3, .exec = ORA},
  {2, ZERO_PAGE, 5, .exec = ASL},
  {},
  {1, ABSOLUTE, 3, .exec = PHP},
  {2, NON_MEMORY, 2, .exec = ORA},
  {1, NON_MEMORY, 2, .exec = ASL, A},
  {}, {},
  {3, ABSOLUTE, 4, .exec = ORA},
  {3, ABSOLUTE, 6, .exec = ASL},
  {},
  //0x1X
  {2, NON_MEMORY, 2, .exec = BPL},
  {2, INDIRECT_INDEXED, 5, .exec = ORA, Y},
  {}, {}, {},
  {2, ZERO_PAGE_INDEXED, 4, .exec = ORA, X},
  {2, ZERO_PAGE_INDEXED, 6, .exec = ASL, X},
  {},
  {1, NON_MEMORY, 2, .exec = CLC},
  {3, ABSOLUTE_INDEXED, 4, .exec = ORA, Y},
  {}, {}, {},
  {3, ABSOLUTE_INDEXED, 4, .exec = ORA, X},
  {3, ABSOLUTE_INDEXED, 7, .exec = ASL, X},
  {},
  //0x2X
  {3, ABSOLUTE, 6, .exec = JSR},
  {2, INDEXED_INDIRECT, 6, .exec = AND, X},
  {},{},
  {2, ZERO_PAGE, 3, .exec = BIT},
  {2, ZERO_PAGE, 3, .exec = AND},
  {2, ZERO_PAGE, 5, .exec = ROL},
  {},
  {1, ABSOLUTE, 4, .exec = PLP},
  {2, NON_MEMORY, 2, .exec = AND},
  {1, NON_MEMORY, 2, .exec = ROL, A},
  {},
  {3, ABSOLUTE, 4, .exec = BIT},
  {3, ABSOLUTE, 4, .exec = AND},
  {3, ABSOLUTE, 6, .exec = ROL},
  {},
  //0x3X
  {2, NON_MEMORY, 2, .exec = BMI},
  {2, INDIRECT_INDEXED, 5, .exec = AND, Y},
  {},{},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = AND, X},
  {2, ZERO_PAGE_INDEXED, 6, .exec = ROL, X},
  {},
  {1, NON_MEMORY, 2, .exec = SEC},
  {3, ABSOLUTE_INDEXED, 4, .exec = AND, Y},
  {},{},{},
  {3, ABSOLUTE_INDEXED, 4, .exec = AND, X},
  {3, ABSOLUTE_INDEXED, 7, .exec = ROL, X},
  {},
  //0x4X
  {1, ABSOLUTE, 6, .exec = RTI},
  {2, INDEXED_INDIRECT, 6, .exec = EOR, X},
  {},{},{},
  {2, ZERO_PAGE, 3, .exec = EOR},
  {2, ZERO_PAGE, 5, .exec = LSR},
  {},
  {1, ABSOLUTE, 3, .exec = PHA},
  {1, NON_MEMORY, 2, .exec = EOR},
  {1, NON_MEMORY, 2, .exec = LSR, A},
  {},
  {3, ABSOLUTE, 3, .exec = JMP},
  {3, ABSOLUTE, 4, .exec = EOR},
  {3, ABSOLUTE, 6, .exec = LSR},
  {},
  //0x5X
  {2, NON_MEMORY, 2, .exec = BVC},
  {2, INDIRECT_INDEXED, 5, .exec = EOR, Y},
  {},{},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = EOR, X},
  {2, ZERO_PAGE_INDEXED, 6, .exec = LSR, X},
  {},
  {1, NON_MEMORY, 2, .exec = CLI},
  {3, ABSOLUTE_INDEXED, 4, .exec = EOR, Y},
  {},{},{},
  {3, ABSOLUTE_INDEXED, 4, .exec = EOR, X},
  {3, ABSOLUTE_INDEXED, 7, .exec = LSR, X},
  {},
  //0x6X
  {1, ABSOLUTE, 6, .exec = RTS},
  {2, INDEXED_INDIRECT, 6, .exec = ADC, X},
  {},{},{},
  {2, ZERO_PAGE, 3, .exec = ADC},
  {2, ZERO_PAGE, 5, .exec = ROR},
  {},
  {1, ABSOLUTE, 4, .exec = PLA},
  {2, NON_MEMORY, 2, .exec = ADC},
  {1, NON_MEMORY, 2, .exec = ROR, A},
  {},
  {3, INDIRECT_INDEXED, 5, .exec = JMP},
  {3, ABSOLUTE, 4, .exec = ADC},
  {3, ABSOLUTE, 6, .exec = ROR},
  {},
  //0x7X
  {2, NON_MEMORY, 2, .exec = BVS},
  {2, INDIRECT_INDEXED, 5, .exec = ADC, Y},
  {},{},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = ADC, X},
  {2, ZERO_PAGE_INDEXED, 6, .exec = ROR, X},
  {},
  {1, NON_MEMORY, 2, .exec = SEI},
  {3, ABSOLUTE_INDEXED, 4, .exec = ADC, Y},
  {},{},{},
  {3, ABSOLUTE_INDEXED, 4, .exec = ADC, X},
  {3, ABSOLUTE_INDEXED, 7, .exec = ROR, X},
  {},
  //0x8X
  {},
  {2, INDEXED_INDIRECT, 6, .exec = STA, X},
  {},{},
  {2, ZERO_PAGE, 3, .exec = STY},
  {2, ZERO_PAGE, 3, .exec = STA},
  {2, ZERO_PAGE, 3, .exec = STX},
  {},
  {1, NON_MEMORY, 2, .exec = DEY},
  {},
  {1, NON_MEMORY, 2, .exec = TXA},
  {},
  {3, ABSOLUTE, 4, .exec = STY},
  {3, ABSOLUTE, 4, .exec = STA},
  {3, ABSOLUTE, 4, .exec = STX},
  {},
  //0x9X
  {2, NON_MEMORY, 2, .exec = BCC},
  {2, INDIRECT_INDEXED, 6, .exec = STA, Y},
  {},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = STY, X},
  {2, ZERO_PAGE_INDEXED, 4, .exec = STA, X},
  {2, ZERO_PAGE_INDEXED, 4, .exec = STX, Y},
  {},
  {1, NON_MEMORY, 2, .exec = TYA},
  {3, ABSOLUTE, 5, .exec = STA, Y},
  {1, NON_MEMORY, 2, .exec = TXS},
  {},{},
  {3, ABSOLUTE, 5, .exec = STA, X},
  {},{},
  //0xAX
  {2, NON_MEMORY, 2, .exec = LDY},
  {2, INDEXED_INDIRECT, 6, .exec = LDA, X},
  {2, NON_MEMORY, 2, .exec = LDX},
  {},
  {2, ZERO_PAGE, 3, .exec = LDY},
  {2, ZERO_PAGE, 3, .exec = LDA},
  {2, ZERO_PAGE, 3, .exec = LDX},
  {},
  {1, NON_MEMORY, 2, .exec = TAY},
  {2, NON_MEMORY, 2, .exec = LDA},
  {1, NON_MEMORY, 2, .exec = TAX},
  {},
  {3, ABSOLUTE, 4, .exec = LDY},
  {3, ABSOLUTE, 4, .exec = LDA},
  {3, ABSOLUTE, 4, .exec = LDX},
  {},
  //0xBX
  {2, NON_MEMORY, 2, .exec = BCS},
  {2, INDIRECT_INDEXED, 5, .exec = LDA, Y},
  {},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = LDY, X},
  {2, ZERO_PAGE_INDEXED, 4, .exec = LDA, X},
  {2, ZERO_PAGE_INDEXED, 4, .exec = LDX, Y},
  {},
  {1, NON_MEMORY, 2, .exec = CLV},
  {2, INDIRECT_INDEXED, 5, .exec = LDA, Y},
  {1, NON_MEMORY, 2, .exec = TSX},
  {},
  {3, ABSOLUTE_INDEXED, 4, .exec = LDY, X},
  {3, ABSOLUTE_INDEXED, 4, .exec = LDA, X},
  {3, ABSOLUTE_INDEXED, 4, .exec = LDX, Y},
  {},
  //0xCX
  {2, ABSOLUTE, 2, .exec = CPY},
  {2, INDEXED_INDIRECT, 6, .exec = CMP, X},
  {},{},
  {2, ZERO_PAGE, 3, .exec = CPY},
  {2, ZERO_PAGE, 3, .exec = CMP},
  {2, ZERO_PAGE, 5, .exec = DEC},
  {},
  {1, NON_MEMORY, 2, .exec = INY},
  {2, NON_MEMORY, 2, .exec = CMP},
  {1, NON_MEMORY, 2, .exec = DEX},
  {},
  {3, ABSOLUTE, 4, .exec = CPY},
  {3, ABSOLUTE, 4, .exec = CMP},
  {3, ABSOLUTE, 3, .exec = DEC},
  {},
  //0xDX
  {2, NON_MEMORY, 2, .exec = BNE},
  {2, INDIRECT_INDEXED, 5, .exec = CMP, Y},
  {},{},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = CMP, X},
  {2, ZERO_PAGE_INDEXED, 6, .exec = DEC, X},
  {},
  {1, NON_MEMORY, 2, .exec = CLD},
  {3, ABSOLUTE_INDEXED, 4, .exec = CMP, Y},
  {},{},{},
  {3, ABSOLUTE, 4, .exec = CMP, X},
  {3, ABSOLUTE, 3, .exec = DEC, X},
  {},
  //0xEX
  {2, NON_MEMORY, 2, .exec = CPX},
  {2, INDEXED_INDIRECT, 6, .exec = SBC, X},
  {},{},
  {2, ZERO_PAGE, 3, .exec = CPX},
  {2, ZERO_PAGE, 3, .exec = SBC},
  {2, ZERO_PAGE, 5, .exec = INC},
  {},
  {1, NON_MEMORY, 2, .exec = INX},
  {2, NON_MEMORY, 2, .exec = SBC},
  {1, NON_MEMORY, 2, .exec = NOP},
  {},
  {3, ABSOLUTE, 4, .exec = CPX},
  {3, ABSOLUTE, 4, .exec = SBC},
  {3, ABSOLUTE, 6, .exec = INC},
  {},
  //0xFX
  {2, NON_MEMORY, 2, .exec = BEQ},
  {2, INDIRECT_INDEXED, 5, .exec = SBC, Y},
  {},{},{},
  {2, ZERO_PAGE_INDEXED, 4, .exec = SBC, X},
  {2, ZERO_PAGE_INDEXED, 6, .exec = INC, X},
  {},
  {1, NON_MEMORY, 2, .exec = SED},
  {3, ABSOLUTE_INDEXED, 4, .exec = SBC, Y},
  {},{},{},
  {3, ABSOLUTE_INDEXED, 4, .exec = SBC, X},
  {3, ABSOLUTE_INDEXED, 7, .exec = INC, X},
  {},
};
