#include "ricoh2a03.hpp"

Ricoh2A03::Ricoh2A03(shared_ptr<Bus> bus): bus{bus}
{
}

~Ricoh2A03()::Ricoh2A03()
{
}

uint8_t Ricoh2A03::read(uint16_t addr)
{
    bus->read(addr);
}

void Ricoh2A03::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}