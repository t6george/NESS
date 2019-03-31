#pragma once
#include <stdbool.h>
#include <sysmem.h>
#include <utils.h>

#define VERSION 0.1


typedef enum {
  C = 0,
  Z,
  I,
  D,
  V = 6,
  N,
} flags;

typedef enum {
  X = 1,
  Y,
  A,
} regIndex;

typedef struct {
  u8 regA;
  u8 regX;
  u8 regY;
  u16 regPC;
  u8 regS;
  u8 regP;
  mainMemory* memory;
  u64 indexRegAddrs[3];
} cpu6502;

cpu6502* powerUpCpu (void);
void powerDownCpu (cpu6502* cpu);
void getVirtualAddress (cpu6502 *cpu, instruction *instr);
void statusFlagSet (cpu6502* cpu, flags flag, bool status);
bool statusFlagGet (cpu6502* cpu, flags flag);
void resetCpu (cpu6502* cpu);
u8 stepInstr (cpu6502* cpu);
