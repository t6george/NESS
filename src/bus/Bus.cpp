#include <Bus.hpp>
#include <RamDevice.hpp>

Bus::Bus() : memory{new RamDevice{0x0000, 0xFFFF}} {}

void Bus::write(uint16_t addr, uint8_t data)
{
    memory->write(addr, data);
}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    return memory->read(addr, readOnly);
}