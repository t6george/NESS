#include <Bus.hpp>
#include <Ram.hpp>

Bus::Bus() : memory{new MemoryDevice{0x800, 0x0000, 0x1FFF}} {}

void Bus::write(uint16_t addr, uint8_t data)
{
    memory->write(addr, data);
}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    return memory->read(addr, readOnly);
}