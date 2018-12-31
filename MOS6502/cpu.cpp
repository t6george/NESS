#include <iostream>
#include <string>
#include <fstream>
#include "../Headers/cpu.hpp"

#define HEADER_SIZE 16
#define INSTROM_SIZE 8192

using namespace std;

CPU::CPU (): regA(0x0), regX(0x0), regY(0x0), regP(0x34), regSP(0xFD), regPC(0x10), isRunning(true) {

}

bool CPU::getCarry () {
  return (regP & 0x1) != 0x0;
}
void CPU::setCarry (bool status) {
  if (status) {
    regP |= 0x1;
  } else {
    regP &= ~(0x1);
  }
}

bool CPU::getZero () {
  return (regP & 0x2) != 0x0;
}
void CPU::setZero (bool status){
  if (status) {
    regP |= 0x2;
  } else {
    regP &= ~(0x2);
  }
}

bool CPU::getIE () {
  return (regP & 0x4) != 0x0;
}
void CPU::setIE (bool status) {
  if (status) {
    regP |= 0x4;
  } else {
    regP &= ~(0x4);
  }
}

bool CPU::getOverflow () {
  return (regP & 0x40) != 0x0;
}
void CPU::setOverflow (bool status) {
  if (status) {
    regP |= 0x40;
  } else {
    regP &= ~(0x40);
  }
}

bool CPU::getNegative () {
  return (regP & 0x80) != 0x0;
}
void CPU::setNegative (bool status) {
  if (status) {
    regP |= 0x80;
  } else {
    regP &= ~(0x80);
  }
}

CPU::~CPU () {}

void CPU::initCartridge (string path) {
  uint32_t byteCount = 0;
  uint32_t frames [6];
  uint32_t ind = 0;
  uint32_t frameI = 0;

  uint8_t headerBytes [HEADER_SIZE];
  ifstream cart (path, ios::binary | ios::in);
  uint32_t byte;
  uint8_t b;

  if (cart.is_open()) {
    while (cart >> b) {
      byte = b;

      if (byteCount < HEADER_SIZE) {
        headerBytes[byteCount] = byte;
      } else if (HEADER_SIZE  == byteCount) {
        this->cartridge = new ROM (headerBytes);
        frames[0] = HEADER_SIZE;

        //TODO: read flag bits
        if (false) { //trainer is present
          frames[1] = 528;
        } else {
          frames[1] = frames[0];
        }

        // rom index
        frames[2] = frames[1] + this->cartridge->prgRomSize;
        frames[3] = frames[2] + this->cartridge->chrRomSize;

        if (false) { //inst rom is present
          frames[4] = INSTROM_SIZE + frames[3];
        } else {
          frames[4] = frames[3];
        }

        if (frames[0] == frames[1]) {
          this->cartridge->prgRomData [ind] = byte;
        } else {
          this->cartridge->trainer [ind] = byte;
        }


      } else {

        for (int i = 0; i < 6; i++) {
          if (frames[i] == byteCount) {
            frameI = i;
            break;
          }
        }

        ind = byteCount - frames[frameI];

        if (0 == frameI) {
          if (frames[0] == frames[1]) {
            this->cartridge->prgRomData [ind] = byte;
          } else {
            this->cartridge->trainer [ind] = byte;
          }

        } else if (1 == frameI) {
          this->cartridge->prgRomData [ind] = byte;

        } else if (2 == frameI) {
          if (frames[2] == frames[3]) {
            if (frames[3] == frames[4]) {
              if (frames[4] != frames[5]) {
                this->cartridge->instPROM [ind] = byte;
              }
            } else {
              this->cartridge->instROM [ind] = byte;
            }

          } else {
            this->cartridge->chrRomData [ind] = byte;
          }

        }

      }
      byteCount++;

    }
    cart.close();

  } else {
    cerr << "Unable to open rom file in location " << path << endl;
  }

}

void CPU::processInstruction (unsigned int instr) {

}

void CPU::printCpuData () {
  cerr << "----------" << endl;
  cerr << "REGA: " << hex << regA << endl;
  cerr << "REGX: " << hex << regX << endl;
  cerr << "REGY: " << hex << regY << endl;
  cerr << "REGP: " << hex << regP << endl << endl;
  cerr << "SP: " << regSP << endl;
  cerr << "PC: " << regPC << endl;
  cerr << "----------" << endl;
}
