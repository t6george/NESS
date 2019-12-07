#include <AddressableDevice.hpp>

uint8_t AddressableDevice::read(uint16_t addr, uint16_t mirror, bool readOnly) const
{
    return getByte(addr & (mirror - 1), readOnly);
}

void AddressableDevice::write(uint16_t addr, uint16_t mirror, uint8_t data)
{
    setByte(addr & (mirror - 1), data);
}