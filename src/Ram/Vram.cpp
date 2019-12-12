#include <Vram.hpp>

VRam::VRam(const uint16_t size) : Ram::Ram{size} {}

uint16_t VRam::mirrorAddress(uint16_t addr, uint16_t mirror)
{
    return addr;
}