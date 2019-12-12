#include <AddressableDevice.hpp>

uint16_t AddressableDevice::mirrorAddress(uint16_t addr, uint16_t mirror)
{
    return addr & (mirror - 1);
}

void AddressableDevice::write(uint16_t addr, uint16_t mirror, uint8_t data)
{
    addr = mirrorAddress(addr, mirror);
    setByte(addr, data);
}

uint8_t AddressableDevice::read(uint16_t addr, uint16_t mirror, bool readOnly)
{
    addr = mirrorAddress(addr, mirror);
    return getByte(addr, readOnly);
}