#ifndef ROM_HPP
#define ROM_HPP

using namespace std;

#define kb8 8192
#define kb16 16384

class ROM {
private:
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


public:
  ROM (unsigned char header [0x10]);
  ~ROM ();

};


#endif
