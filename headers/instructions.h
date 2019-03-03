#include "sysmem.h"

typedef struct {
  u8 size;
  u8* auxBytes;
  addressingMode addrMode;
  u8 cycles;
  char* mnemonic;
} instruction;


instruction instructionMap [0x100] {
  {0, NULL, NON_MEMORY, 7, "BRK"},
  {2, NULL, INDEXED_INDIRECT, 6, "ORA INDIRECT X"},
  {1, NULL, ZERO_PAGE, 3, "ORA ZEROPAGE"},
  {1, NULL, ZERO_PAGE, 5, "ASL ZEROPAGE"},

  {0, NULL, ABSOLUTE, 7, "PHP"},
  {1, NULL, NON_MEMORY, 7, "BRK"},
  {1, NULL, NON_MEMORY, 7, "BRK"},
  {1, NULL, NON_MEMORY, 7, "BRK"},
  {1, NULL, NON_MEMORY, 7, "BRK"},
  {1, NULL, NON_MEMORY, 7, "BRK"},
};
