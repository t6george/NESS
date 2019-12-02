#include <AddressableDevice.hpp>

AddressableDevice::AddressableDevice(const uint16_t addrBase, const uint16_t addrEnd) : addrBase{addrBase}, addrEnd{addrEnd} {}

AddressableDevice::AddressableDevice(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd) : addrBase{addrBase}, addrEnd{addrEnd}, contents{std::vector<uint8_t>(size, 0x00)} {}

uint8_t AddressableDevice::readFrom(uint16_t addr, bool readOnly) const
{
    uint8_t data = 0x00;
    // Bitwise and performs the address mirroring
    if (addr >= addrBase && addr < addrEnd)
    {
        data = getByte(addr & (contents.size() - 1), readOnly);
    }

    return data;
}

bool AddressableDevice::writeTo(uint16_t addr, uint8_t data)
{
    bool success = false;
    // Bitwise and performs the address mirroring
    if (addr >= addrBase && addr <= addrEnd)
    {
        setByte(addr & (contents.size() - 1), data);
        success = true;
    }

    return success;
}