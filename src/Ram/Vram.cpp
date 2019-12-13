#include <Vram.hpp>

VRam::VRam(const uint16_t size, GamePak::MirrorMode mMode) 
    : Ram::Ram{size}, mMode{mMode} {}

uint16_t VRam::mirrorAddress(uint16_t addr, uint16_t mirror)
{
    uint16_t base = 0x0000;

    if (mMode == GamePak::VERTICAL && 
        ((addr >= 0x0400 && addr <= 0x07FF) || (addr >= 0x0C00 && addr <= 0x0FFF)))
    {
        base = 0x0400;
    }
    else if (mMode == GamePak::HORIZONTAL &&
        addr >= 0x0000 && addr <= 0x07FF)
    {
        base = 0x0400;
    }

    return base + addr & 0x03FF;
}