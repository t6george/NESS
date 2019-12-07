#pragma once
#include <cstdint>

/*********** PPU Addressing Constants ***********/
namespace PPU
{

namespace CARTRIDGE
{
const uint16_t Base = 0x0000;
const uint16_t Limit = 0x1FFF;
const uint16_t Mirror = 0x0000;
}; // namespace CARTRIDGE

}; // namespace PPU

/*********** CPU Addressing Constants ***********/
namespace CPU
{

namespace RAM
{
const uint16_t Base = 0x0000;
const uint16_t Limit = 0x1FFF;
const uint16_t Mirror = 0x0800;
}; // namespace RAM

namespace PPU
{
const uint16_t Base = 0x2000;
const uint16_t Limit = 0x3FFF;
const uint16_t Mirror = 0x0008;
}; // namespace PPU

namespace CARTRIDGE
{
const uint16_t Base = 0x4020;
const uint16_t Limit = 0xFFFF;
const uint16_t Mirror = 0x0000;
}; // namespace CARTRIDGE

}; // namespace CPU