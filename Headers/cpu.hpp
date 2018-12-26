#ifndef CPU_HPP
#define CPU_HPP

#include "./rom.hpp"

using namespace std;

class CPU {
private:

  ROM * cartridge = NULL;

  unsigned char regA = 0;
  unsigned char regX = 0;
  unsigned char regY = 0;

  unsigned char sp;
  unsigned short pc;

public:
  CPU ();
  ~CPU ();

  void initCartridge (string path);
  void processInstruction (unsigned int instr);
  void printPrgRom ();
};


#endif
