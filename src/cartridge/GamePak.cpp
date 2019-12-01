#include <GamePak.hpp>

GamePak::GamePak(const uint16_t prgSize, const uint16_t chrSize): 
prgMem{vector<uint8_t>(prgSize, 0x00}, chrMem{vector<uint8_t>(chrSize, 0x00} {}

void GamePak::write(uint16_t addr, uint8_t data)
{

}

uint8_t GamePak::read(uint16_t addr, bool readOnly) const
{
    return 0;
}