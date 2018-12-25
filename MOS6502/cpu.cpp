#include <iostream>
#include <string>
#include <fstream>

#define HEADER_SIZE 16
#define INSTROM_SIZE 8192

#include "../Headers/cpu.hpp"

using namespace std;

CPU::CPU () {}
CPU::~CPU () {}


void CPU::initCartridge (string path) {
  unsigned int byteCount = 0;
  unsigned int frames [6];
  unsigned int ind = 0;
  int frameI = 0;

  unsigned char headerBytes [HEADER_SIZE];
  ifstream cartridge (path, ios::binary | ios::in);
  unsigned short byte;
  unsigned char b;

  if (cartridge.is_open()) {
    while (cartridge >> b) {
      byte = b;
      if (byteCount < HEADER_SIZE) {
        headerBytes[byteCount] = byte;
      } else if (HEADER_SIZE  == byteCount) {
        this->cartridge = new ROM (headerBytes);
        frames[0] = HEADER_SIZE;

        if (false) { //trainer is present
          frames[1] = 528;
        } else {
          frames[1] = frames[0];
        }

        // rom index
        frames[2] = frames[1] + this->cartridge.prgRomSize;
        frames[3] = frames[2] + this->cartridge.chrRomSize;

        if (false) { //inst rom is present
          frames[4] = INSTROM_SIZE + frames[3];
        } else {
          frames[1] = frames[3];
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
                this->cartridge->instrPROM [ind] = byte;
              }
            } else {
              this->cartridge->instrROM [ind] = byte;
            }
          } else {
            this->cartridge->chrRomData [ind] = byte;
          }
        }

      }
      byteCount++;

    }
    cartridge.close();
  } else {
    cerr << "Unable to open rom file in location " << path << endl;
  }

}

void CPU::processInstruction (unsigned int instr) {

}
