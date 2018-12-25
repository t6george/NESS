#include <iostream>

#include "../Headers/rom.hpp"

using namespace std;

ROM::ROM (unsigned char header [0x10]) {
  prgRomSize = header[4] * KB_SIZE * 16;
  chrRomSize = header[5] * KB_SIZE * 8;
  prgRamSize = header[8] * KB_SIZE * 8;

  flags6 = header[6];
  flags7 = header[7];
  flags9 = header[9];
  flags10 = header[10];

  trainer = new unsigned char [KB_SIZE/2];
  prgRomData = new unsigned char [prgRomSize];
  chrRomData = new unsigned char [chrRomSize];
  instROM = new unsigned char [KB_SIZE * 8];
  instPROM = new unsigned char [32];


}

ROM::~ROM () {
  delete[] trainer;
  delete[] prgRomData;
  delete[] chrRomData;
  delete[] instROM;
  delete[] instPROM;
}
