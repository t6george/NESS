#include <RicohRP2C02.hpp>

RicohRP2C02::RicohRP2C02(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd): 
AddressableDevice::AddressableDevice(size, addrBase, addrEnd) {}

void RicohRP2C02::setByte(uint16_t addr, uint8_t data)
{

}

uint8_t RicohRP2C02::getByte(uint16_t addr, bool readOnly) const
{
    return 0;
}