#include <iostream>

#include "../Headers/rom.hpp"

using namespace std;

ROM::ROM (unsigned char header [0x10]) {
  prgRomSize = header[4] * kb16;
  chrRomSize = header[5] * kb8;
  prgRamSize = header[8] * kb8;

  flags6 = header[6];
  flags7 = header[7];
  flags9 = header[9];
  flags10 = header[10];

  prgRomData = new unsigned char [prgRomSize];
  chrRomData = new unsigned char [chrRomSize];


}

ROM::~ROM () {
  delete[] trainer;
  delete[] prgRomData;
  delete[] chrRomData;
  delete[] instROM;
  delete[] instPROM;
}
