#include <PaletteRam.hpp>

PaletteRam::PaletteRam(const uint16_t size) : Ram::Ram{size} {}

uint16_t PaletteRam::mirrorAddress(uint16_t addr, uint16_t mirror)
{
    addr = AddressableDevice::mirrorAddress(addr, mirror);

    if ((addr & 0x0003) == 0x0000)
        addr &= 0x000F;

    return addr;
}