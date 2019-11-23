#include "ramdevice.hpp"

RamDevice::RamDevice(constexpr uint16_t addrBase, constexpr uint16_t addrRange) : addrBase{addrBase}, addrRange{addrRange}
{
    for (int i = 0; i < addrRange; ++i)
        contents[i] = 0x00;
}

uint8_t RamDevice::read(uint16_t addr, bool readOnly = false) const
{
    if(addr >= addrBase && addr - addrBase < addrRange)
        return contents[addr];

    return 0x00;
}

void RamDevice::write(uint16_t addr, uint8_t data)
{
    if(addr >= addrBase && addr - addrBase < addrRange)
        contents[addr] = data;
}