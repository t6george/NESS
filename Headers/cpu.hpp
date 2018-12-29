#ifndef CPU_HPP
#define CPU_HPP

#include "./rom.hpp"
// #include <unordered_map>

using namespace std;

class CPU {
private:

  ROM * cartridge = NULL;

  unsigned char regA = 0;
  unsigned char regX = 0;
  unsigned char regY = 0;

  unsigned char sp;
  unsigned short pc;

  unsigned char instrLengths [0xFF];

public:
  CPU ();
  ~CPU ();

  void initCartridge (string path);
  void processInstruction (unsigned int instr);
  void printPrgRom ();
};


#endif
