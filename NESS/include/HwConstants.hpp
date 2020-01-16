#pragma once
#include <cstdint>

/*********** PPU Addressing Constants ***********/
namespace PPU
{

namespace CARTRIDGE
{
constexpr uint16_t Base = 0x0000;
constexpr uint16_t Limit = 0x1FFF;
constexpr uint16_t Mirror = 0x2000;

namespace CHR
{
constexpr uint16_t PartitionSize = 0x1000;
constexpr uint16_t NumTiles = 0x0100;
};

}; // namespace CARTRIDGE

namespace PALETTE
{
constexpr uint16_t Base = 0x3F00;
constexpr uint16_t Limit = 0x3FFF;
constexpr uint16_t Mirror = 0x0020;
constexpr uint16_t Size = 0x0020;
};

namespace VRAM
{
constexpr uint16_t Base = 0x2000;
constexpr uint16_t Limit = 0x3EFF;
constexpr uint16_t Mirror = 0x1000;
constexpr uint16_t Size = 0x0800;
}; // namespace VRAM

}; // namespace PPU

/*********** CPU Addressing Constants ***********/
namespace CPU
{

namespace RAM
{
constexpr uint16_t Base = 0x0000;
constexpr uint16_t Limit = 0x1FFF;
constexpr uint16_t Mirror = 0x0800;
constexpr uint16_t Size = 0x0800;
}; // namespace RAM

namespace PPU
{
constexpr uint16_t Base = 0x2000;
constexpr uint16_t Limit = 0x3FFF;
constexpr uint16_t Mirror = 0x0008;
}; // namespace PPU

namespace CARTRIDGE
{
constexpr uint16_t Base = 0x4020;
constexpr uint16_t Limit = 0xFFFF;
constexpr uint16_t Mirror = 0xBFE0;
}; // namespace CARTRIDGE

}; // namespace CPU

namespace CARTRIDGE
{
constexpr uint16_t PrgBankSize = 0x4000;
constexpr uint16_t ChrBankSize = 0x2000;
}; // namespace CARTRIDGE

/*********** Graphical Constants ***********/

namespace DISPLAY
{
constexpr uint16_t Height = 0x00F0;
constexpr uint16_t Width = 0x0100;
constexpr uint16_t ColorCount = 0x0040;
constexpr uint16_t PixelDim = 0x0002;
constexpr uint32_t PixelOpacity = 0xFF000000;
}; // namespace DISPLAY