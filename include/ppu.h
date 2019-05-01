typedef enum {
  PPUCTRL,
  PPUMASK,
  PPUSTATUS,
  OAMADDR,
  OAMDATA,
  PPUSCROLL,
  PPUADDR,
  PPUDATA,
} ppuReg;

typedef struct {
  u8 *registerBase;

} ppuNES;


ppuNES* powerUpPpu (ppuNES *ppu, mainMemory* memory);
u8 ppuFlagGet (ppuNES* ppu, ppuReg reg, u8 lsb, u8 fieldWidth);
void ppuFlagSet (ppuNES* ppu, ppuReg reg, u8 lsb, u8 fieldWidth, u8 val);
