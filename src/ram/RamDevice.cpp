#include <RamDevice.hpp>

RamDevice::RamDevice(const uint16_t addrBase, const uint16_t addrRange) : addrBase{addrBase}, contents{std::vector<uint8_t>(addrRange, 0x00)} {}

uint8_t RamDevice::read(uint16_t addr, bool readOnly) const
{
    if (addr >= addrBase && addr - addrBase < contents.size())
        return contents[addr];

    return 0x00;
}

void RamDevice::write(uint16_t addr, uint8_t data)
{
    if (addr >= addrBase && addr - addrBase < contents.size())
        contents[addr] = data;
}