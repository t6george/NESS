#ifndef ROM_HPP
#define ROM_HPP

using namespace std;

#define KB_SIZE 1024

struct ROM {
  uint32_t prgRomSize;
  uint32_t chrRomSize;
  uint32_t prgRamSize;

  uint8_t flags6;
  uint8_t flags7;
  uint8_t flags9;
  uint8_t flags10;

  uint8_t * trainer = nullptr;
  uint8_t * prgRomData = nullptr;
  uint8_t * chrRomData = nullptr;
  uint8_t * instROM = nullptr;
  uint8_t * instPROM = nullptr;

  ROM (uint8_t header [0x10]);
  ~ROM ();
  void printRomData ();

};


#endif
