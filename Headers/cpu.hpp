#ifndef CPU_HPP
#define CPU_HPP

#include "./rom.hpp"

class CPU {
private:

  ROM * cartridge = nullptr;

  RAM * mem = nullptr;

  PPU * ppu = nullptr;

  uint8_t regA;
  uint8_t regX;
  uint8_t regY;

  uint8_t regP;

  uint8_t regSP;
  uint16_t regPC;

public:
  bool isRunning;

  CPU ();
  ~CPU ();

  void initCartridge (string path);
  void readRom ();
  void printCpuData ();

  bool getCarry ();
  void setCarry (bool status);

  bool getZero ();
  void setZero (bool status);

  bool getIE ();
  void setIE (bool status);

  bool getOverflow ();
  void setOverflow (bool status);

  bool getNegative ();
  void setNegative (bool status);

  bool getDecimal ();
  void setDecimal (bool status);

  uint8_t readRamByte (uint16_t addr);
  void writeRamByte (uint16_t addr, uint8_t value);

};


#endif
