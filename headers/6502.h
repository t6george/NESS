#include <stdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;

typedef struct {
  u8 regA;
  u8 regX;
  u8 regY;
  u16 regPC;
  u8 regS;
  u8 regP;
  mainMemory* memory;
} cpu6502;

cpu6502* powerUpCpu (void);
void powerDownCpu (cpu6502* cpu);
void resetCpu (cpu6502* cpu);
