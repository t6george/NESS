#include <Ram.hpp>
#include <cassert>

MemoryDevice::MemoryDevice(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd) : 
addrBase{addrBase}, addrEnd{addrEnd}, contents{std::vector<uint8_t>(size, 0x00)} 
{
    // Ensure that memory size is a power of 2
    assert(size & (size - 1) == 0);
}

void Ram::setByte(uint16_t addr, uint8_t data) override
{
    contents[addr] = data;
}

uint8_t Ram::getByte(uint16_t addr, bool readOnly) const override
{
    return contents[addr];
}