#ifndef CPU_HPP
#define CPU_HPP

#include "./rom.hpp"

using namespace std;

#define INSTR_SIZE 8

class CPU {
private:

  ROM * cartridge = NULL;

  unsigned char regA = 0;
  unsigned char regX = 0;
  unsigned char regY = 0;



public:
  CPU ();
  ~CPU ();

  void initCartridge (string path);
  void processInstruction (unsigned int instr);
};


#endif
