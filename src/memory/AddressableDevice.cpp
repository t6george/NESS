#include <RamDevice.hpp>
#include <cassert>

AddressableDevice::AddressableDevice(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd) : 
addrBase{addrBase}, addrEnd{addrEnd}, contents{std::vector<uint8_t>(size, 0x00)} 
{
    // Ensure that memory size is a power of 2
    assert(size & (size - 1) == 0);
}

uint8_t AddressableDevice::read(uint16_t addr, bool readOnly) const
{
    uint8_t data = 0x00;
    // Bitwise and performs the address mirroring
    if (addr >= addrBase && addr - addrBase < addrRange)
    {
        data = getByte(addr & (contents.size() - 1), readOnly);
    }

    return data;
}

void AddressableDevice::write(uint16_t addr, uint8_t data)
{
    // Bitwise and performs the address mirroring
    if (addr >= addrBase && addr - addrBase < addrRange)
    {
        setByte(addr & (contents.size() - 1), data);
    }        
}