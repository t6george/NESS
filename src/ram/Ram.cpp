#include <Ram.hpp>
#include <cassert>

Ram::Ram(const uint16_t size) : 
contents{std::vector<uint8_t>(size, 0x00)} 
{
    // Ensure that memory size is a power of 2
    assert((size & (size - 1)) == 0);
}

uint8_t Ram::read(uint16_t addr, bool readOnly) const
{
    return memory[addr];
}

void Ram::write(uint16_t addr, uint8_t data)
{
    memory[addr] = data;    
}