#include <Ram.hpp>
#include <cassert>

Ram::Ram(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd): 
AddressableDevice::AddressableDevice(size, addrBase, addrEnd) {}

void Ram::setByte(uint16_t addr, uint8_t data)
{
    contents[addr] = data;
}

uint8_t Ram::getByte(uint16_t addr, bool readOnly) const
{
    return contents[addr];
}