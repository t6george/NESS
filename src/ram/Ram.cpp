#include <Ram.hpp>

Ram::Ram(const uint16_t size) : 
contents{std::vector<uint8_t>(size, 0x00)} {}

uint8_t Ram::read(uint16_t addr, bool readOnly) const
{
    return memory[addr];
}

void Ram::write(uint16_t addr, uint8_t data)
{
    memory[addr] = data;    
}