#ifndef ROM_HPP
#define ROM_HPP

using namespace std;

#define KB_SIZE 1024

struct ROM {
  unsigned int prgRomSize;
  unsigned int chrRomSize;
  unsigned int prgRamSize;

  unsigned char flags6;
  unsigned char flags7;
  unsigned char flags9;
  unsigned char flags10;

  unsigned char * trainer = nullptr;
  unsigned char * prgRomData = nullptr;
  unsigned char * chrRomData = nullptr;
  unsigned char * instROM = nullptr;
  unsigned char * instPROM = nullptr;

  ROM (unsigned char header [0x10]);
  ~ROM ();

};


#endif
