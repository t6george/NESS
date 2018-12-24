#include <iostream>
#include <string>
#include <fstream>
#include <vector>


#include "../Headers/cpu.hpp"
#include "rom.cpp"

using namespace std;

CPU::CPU () {}
CPU::~CPU () {}


void CPU::initCartridge (string path) {
  unsigned int byteCount = 0;
  unsigned char header [0x10];
  ifstream cartridge (path, ios::binary | ios::in);
  unsigned short byte;
  unsigned char b;

  if (cartridge.is_open()) {
    while (cartridge >> b) {
      byte = b;
      if (byteCount < 0x10) {
        header[byteCount] = byte;
        byteCount++;
      } else if (0x10 == byteCount) {
        
        byteCount++;
      } else () {

      }

    }
    cartridge.close();
  } else {
    cerr << "Unable to open rom file in location " << filepath << endl;
  }

}

void CPU::processInstruction (unsigned int instr) {

}
