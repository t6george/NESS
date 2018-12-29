#include <iostream>
#include <string>
#include <fstream>
#include "../Headers/cpu.hpp"

#define HEADER_SIZE 16
#define INSTROM_SIZE 8192

using namespace std;

CPU::CPU (): pc(HEADER_SIZE) {
  instrLengths[0x00] = 1;
  instrLengths[0x01] = 2;
  instrLengths[0x05] = 2;
  instrLengths[0x06] = 2;
  instrLengths[0x08] = 1;
  instrLengths[0x09] = 2;
  instrLengths[0x0A] = 1;
  instrLengths[0x0D] = 3;
  instrLengths[0x0E] = 3;

  instrLengths[0x10] = 2;
  instrLengths[0x11] = 2;
  instrLengths[0x15] = 2;
  instrLengths[0x16] = 2;
  instrLengths[0x18] = 1;
  instrLengths[0x19] = 3;
  instrLengths[0x1D] = 3;
  instrLengths[0x1E] = 3;

  instrLengths[0x20] = 3;
  instrLengths[0x21] = 2;
  instrLengths[0x24] = 2;
  instrLengths[0x25] = 2;
  instrLengths[0x26] = 2;
  instrLengths[0x28] = 1;
  instrLengths[0x29] = 2;
  instrLengths[0x2A] = 1;
  instrLengths[0x2C] = 3;
  instrLengths[0x2D] = 3;
  instrLengths[0x2E] = 3;

  instrLengths[0x30] = 2;
  instrLengths[0x31] = 2;
  instrLengths[0x35] = 2;
  instrLengths[0x36] = 2;
  instrLengths[0x38] = 1;
  instrLengths[0x39] = 3;
  instrLengths[0x3D] = 3;
  instrLengths[0x3E] = 3;

  instrLengths[0x40] = 1;
  instrLengths[0x41] = 2;
  instrLengths[0x45] = 2;
  instrLengths[0x46] = 2;
  instrLengths[0x48] = 1;
  instrLengths[0x49] = 2;
  instrLengths[0x4A] = 1;
  instrLengths[0x4C] = 3;
  instrLengths[0x4D] = 3;
  instrLengths[0x4E] = 3;

  instrLengths[0x50] = 2;
  instrLengths[0x51] = 2;
  instrLengths[0x55] = 2;
  instrLengths[0x56] = 2;
  instrLengths[0x58] = 1;
  instrLengths[0x59] = 3;
  instrLengths[0x5D] = 3;
  instrLengths[0x5E] = 3;

  instrLengths[0x60] = 1;
  instrLengths[0x61] = 2;
  instrLengths[0x65] = 2;
  instrLengths[0x66] = 2;
  instrLengths[0x68] = 1;
  instrLengths[0x69] = 2;
  instrLengths[0x6A] = 1;
  instrLengths[0x6C] = 3;
  instrLengths[0x6D] = 3;
  instrLengths[0x6E] = 3;

  instrLengths[0x70] = 2;
  instrLengths[0x71] = 2;
  instrLengths[0x75] = 2;
  instrLengths[0x76] = 2;
  instrLengths[0x78] = 1;
  instrLengths[0x79] = 3;
  instrLengths[0x7D] = 3;
  instrLengths[0x7E] = 3;

  instrLengths[0x81] = 2;
  instrLengths[0x84] = 2;
  instrLengths[0x85] = 2;
  instrLengths[0x86] = 2;
  instrLengths[0x88] = 1;
  instrLengths[0x8A] = 1;
  instrLengths[0x8C] = 3;
  instrLengths[0x8D] = 3;
  instrLengths[0x8E] = 3;

  instrLengths[0x90] = 2;
  instrLengths[0x91] = 2;
  instrLengths[0x94] = 2;
  instrLengths[0x95] = 2;
  instrLengths[0x96] = 2;
  instrLengths[0x98] = 1;
  instrLengths[0x99] = 3;
  instrLengths[0x9A] = 1;
  instrLengths[0x9D] = 3;

  instrLengths[0xA0] = 2;
  instrLengths[0xA1] = 2;
  instrLengths[0xA2] = 2;
  instrLengths[0xA4] = 2;
  instrLengths[0xA5] = 2;
  instrLengths[0xA6] = 2;
  instrLengths[0xA8] = 1;
  instrLengths[0xA9] = 2;
  instrLengths[0xAA] = 1;
  instrLengths[0xAC] = 3;
  instrLengths[0xAD] = 3;
  instrLengths[0xAE] = 3;

  instrLengths[0xB0] = 2;
  instrLengths[0xB1] = 2;
  instrLengths[0xB4] = 2;
  instrLengths[0xB5] = 2;
  instrLengths[0xB6] = 2;
  instrLengths[0xB8] = 1;
  instrLengths[0xB9] = 3;
  instrLengths[0xBA] = 1;
  instrLengths[0xBC] = 3;
  instrLengths[0xBD] = 3;
  instrLengths[0xBE] = 3;

  instrLengths[0xC0] = 2;
  instrLengths[0xC1] = 2;
  instrLengths[0xC4] = 2;
  instrLengths[0xC5] = 2;
  instrLengths[0xC6] = 2;
  instrLengths[0xC8] = 1;
  instrLengths[0xC9] = 2;
  instrLengths[0xCA] = 1;
  instrLengths[0xCC] = 3;
  instrLengths[0xCD] = 3;
  instrLengths[0xCE] = 3;

  instrLengths[0xD0] = 2;
  instrLengths[0xD1] = 2;
  instrLengths[0xD5] = 2;
  instrLengths[0xD6] = 2;
  instrLengths[0xD8] = 1;
  instrLengths[0xD9] = 3;
  instrLengths[0xDD] = 3;
  instrLengths[0xDE] = 3;

  instrLengths[0xE0] = 2;
  instrLengths[0xE1] = 2;
  instrLengths[0xE4] = 2;
  instrLengths[0xE5] = 2;
  instrLengths[0xE6] = 2;
  instrLengths[0xE8] = 1;
  instrLengths[0xE9] = 2;
  instrLengths[0xEA] = 1;
  instrLengths[0xEC] = 3;
  instrLengths[0xED] = 3;
  instrLengths[0xEE] = 3;

  instrLengths[0xF0] = 2;
  instrLengths[0xF1] = 2;
  instrLengths[0xF5] = 2;
  instrLengths[0xF6] = 2;
  instrLengths[0xF8] = 1;
  instrLengths[0xF9] = 3;
  instrLengths[0xFD] = 3;
  instrLengths[0xFE] = 3;

}

CPU::~CPU () {}

void CPU::initCartridge (string path) {
  unsigned int byteCount = 0;
  unsigned int frames [6];
  unsigned int ind = 0;
  int frameI = 0;

  unsigned char headerBytes [HEADER_SIZE];
  ifstream cart (path, ios::binary | ios::in);
  unsigned short byte;
  unsigned char b;

  if (cart.is_open()) {
    while (cart >> b) {
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

void CPU::printPrgRom () {
  // for (int i = 0; i < 3; i++) {
  //   int a = this->cartridge->prgRomData[i];
  //   cout << hex << a << endl;
  // }
}
