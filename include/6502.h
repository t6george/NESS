#pragma once
#include <utils.h>
#include <stdbool.h>
#include <sysmem.h>
struct instruction;
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

typedef enum {
  NMI,
  RESET,
  IRQ,
} interruptT;

typedef struct {
  u8 regA;
  u8 regX;
  u8 regY;
  u16 regPC;
  u8 regS;
  u8 regP;
  mainMemory* memory;
  u64 indexRegAddrs[3];
  u8 interruptV: 3;
} cpu6502;

#include <instructions.h>
cpu6502* powerUpCpu (void);
void powerDownCpu (cpu6502* cpu);
void getVirtualAddress (cpu6502 *cpu, struct instruction *instr);
void cpuFlagSet (cpu6502* cpu, flags flag, bool status);
bool cpuFlagGet (cpu6502* cpu, flags flag);
void resetCpu (cpu6502* cpu);
void interruptRq (cpu6502 *cpu, interruptT irqType);
void interruptSrvice (cpu6502 *cpu);
void nmiHandler (cpu6502 *cpu);
void resetHandler (cpu6502 *cpu);
void irqHandler (cpu6502 *cpu, u8 bFlag);
u8 executeInstr (cpu6502* cpu);
