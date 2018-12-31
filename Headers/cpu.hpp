#ifndef CPU_HPP
#define CPU_HPP

#include "./rom.hpp"
// #include <unordered_map>

using namespace std;

class CPU {
private:

  ROM * cartridge = NULL;

  uint8_t regA;
  uint8_t regX;
  uint8_t regY;
  
  uint8_t regP;

  uint8_t regSP;
  uint16_t regPC;

  uint8_t instrLengths [0xFF];

public:
  bool isRunning;

  CPU ();
  ~CPU ();

  void initCartridge (string path);
  void processInstruction (uint32_t instr);
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

};


#endif
