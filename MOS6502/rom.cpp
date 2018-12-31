#include <iostream>

#include "../Headers/rom.hpp"

using namespace std;

ROM::ROM (uint8_t header [0x10]) {
  prgRomSize = header[4] * KB_SIZE * 16;
  chrRomSize = header[5] * KB_SIZE * 8;
  prgRamSize = header[8] * KB_SIZE * 8;

  flags6 = header[6];
  flags7 = header[7];
  flags9 = header[9];
  flags10 = header[10];

  trainer = new uint8_t [KB_SIZE/2];
  prgRomData = new uint8_t [prgRomSize];
  instROM = new uint8_t [KB_SIZE * 8];
  instPROM = new uint8_t [32];

  if (0 < chrRomSize) {
    chrRomData = new uint8_t [chrRomSize];
  }


}

ROM::~ROM () {
  delete[] trainer;
  delete[] prgRomData;
  delete[] chrRomData;
  delete[] instROM;
  delete[] instPROM;
}

void ROM::printRomData () {
  cerr << "~~~~~~~~~~" << endl;
  cerr << "prgRomSize: " << hex << prgRomSize << endl;
  cerr << "chrRomSize: " << hex << chrRomSize << endl;
  cerr << "prgRamSize: " << hex << prgRamSize << endl << endl;

  cerr << "flags6: " << hex << flags6 << endl;
  cerr << "flags7: " << hex << flags7 << endl;
  cerr << "flags9: " << hex << flags9 << endl;
  cerr << "flags10: " << hex << flags10 << endl << endl;

  cerr << "trainer: " << trainer[0] << endl;
  cerr << "prgRomData: " << prgRomData[0] << endl;
  cerr << "instROM: " << instROM[0] << endl;
  cerr << "instPROM: " << instPROM[0] << endl;

  if (0 < chrRomSize) {
    cerr << "chrRomData: " << chrRomData[0] << endl;
  }


  cerr << "~~~~~~~~~~" << endl;
}
