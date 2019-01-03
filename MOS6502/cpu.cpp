#include <iostream>
#include <string>
#include <fstream>
#include "../Headers/cpu.hpp"

#define HEADER_SIZE 16
#define KB_SIZE 1024
#define RAM_END 0x1FFF
#define PPU_END 0x3FFF
#define APU_IO_END 0x401F
#define ROM_END 0xFFFF


using namespace std;

CPU::CPU (): regA(0x0), regX(0x0), regY(0x0), regP(0x34), regSP(0xFD), regPC(0x10), isRunning(true) {
  mem = new RAM ();
  ppu = new PPU ();
}

uint8_t CPU::readRamByte (uint16_t addr) {
  uint8_t b;
  if (addr < RAM_END) {
    b = mem->sendByte (addr);
  } else if (addr < PPU_END) {
    b = ppu->readReg (addr - 1 - RAM_END);
  } else if (addr <= APU_IO_END) {
    //ppu->writeReg (addr - 1 - RAM_END,value);
  } else if (addr <= ROM_END) {
    //ppu->writeReg (addr - 1 - RAM_END,value);
  }
  
  return b;
}
void CPU::writeRamByte (uint16_t addr, uint8_t value) {
  if (addr <= RAM_END) {
    mem->recieveByte (addr,value);
  } else if (addr <= PPU_END) {
    ppu->writeReg (addr - 1 - RAM_END,value);
  } else if (addr <= APU_IO_END) {
    //ppu->writeReg (addr - 1 - RAM_END,value);
  } else if (addr <= ROM_END) {
    //ppu->writeReg (addr - 1 - RAM_END,value);
  }
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

bool CPU::getDecimal () {
  return (regP & 0x8) != 0x0;
}
void CPU::setDecimal (bool status) {
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
          frames[4] = KB_SIZE * 8 + frames[3];
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

void CPU::readRom () {
  while (isRunning) {
    uint8_t opcode = this->cartridge->prgRomData[regPC];
    uint16_t address;

    switch (opcode) {
      case (0x78): {
        setIE (true);
        regPC += 1;
        break;
      } case (0x8D): {
        address = (this->cartridge->prgRomData[regPC + 2] << 1) + this->cartridge->prgRomData[regPC + 1];
        writeRamByte (address, regA);
        regPC += 3;
        break;
      } case (0xA9): {
        regA = this->cartridge->prgRomData[regPC + 1];
        regPC += 2;
        break;
      } case (0xD8): {
        setDecimal (false);
        regPC += 1;
        break;
      } default: {
        cerr << "Unidentified Opcode: " << hex << opcode << endl;
      }
    }

  }
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
