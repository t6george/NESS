#include <Bus.hpp>
#include <Ram.hpp>
#include <RicohRP2C02.hpp>

void Bus::write(uint16_t addr, uint8_t data)
{
    delegate(addr)->write(addr, data);
}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    return delegate(addr)->read(addr, readOnly);
}