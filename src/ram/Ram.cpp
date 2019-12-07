#include <Ram.hpp>

Ram::Ram(const uint16_t size) : contents{std::vector<uint8_t>(size, 0x00)} {}

void Ram::setByte(uint16_t addr, uint8_t data)
{
    contents[addr] = data;
}

uint8_t Ram::getByte(uint16_t addr, bool readOnly) const
{
    return contents[addr];
}