#include <iostream>
#include <string>
#include <fstream>

#include "../Headers/cpu.hpp"

using namespace std;

CPU::CPU () {}
CPU::~CPU () {}


void CPU::initCartridge (string path) {
  unsigned int byteCount = 0;
  unsigned char headerBytes [0x10];
  ifstream cartridge (path, ios::binary | ios::in);
  unsigned short byte;
  unsigned char b;

  if (cartridge.is_open()) {
    while (cartridge >> b) {
      byte = b;
      if (byteCount < 0x10) {
        headerBytes[byteCount] = byte;
        byteCount++;
      } else if (0x10 == byteCount) {
        this->cartridge = new ROM (headerBytes);
        byteCount++;
      } else {
        byteCount++;
      }

    }
    cartridge.close();
  } else {
    cerr << "Unable to open rom file in location " << path << endl;
  }

}

void CPU::processInstruction (unsigned int instr) {

}
