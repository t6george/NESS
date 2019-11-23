#include "bus.hpp"

Bus::Bus(): memory{make_unique(new RamDevice{0x0000, 0xFFFF})},
    A{0x00}, X{0x00}, Y{0x00}, SP{0x00}, PC{0x0000}, S{0x00}
{
}

~Bus::Bus()
{
}

void write(uint16_t addr, uint8_t data)
{
    RamDevice->write(addr, data);
}

uint8_t read(uint16_t addr, bool readOnly = false)
{
    return RamDevice->read(addr, readOnly);
}