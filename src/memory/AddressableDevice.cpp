#include <AddressableDevice.hpp>
#include <cassert>

AddressableDevice::AddressableDevice(const uint16_t addrBase, const uint16_t addrEnd)
    : addrBase{addrBase}, addrEnd{addrEnd}, mirrorSize{0}, requestSrc{Request::CPU} {}

AddressableDevice::AddressableDevice(const std::vector<uint16_t> &sizes, const uint16_t addrBase, const uint16_t addrEnd)
    : addrBase{addrBase}, addrEnd{addrEnd}, mirrorSize{0}, contents{std::vector<std::vector<uint8_t>>(sizes.size())}
{
    for (size_t i = 0; i < sizes.size(); ++i)
    {
        contents[i] = std::vector<uint8_t>(sizes[i], 0x00);
        mirrorSize += sizes[i];
    }

    // we assume that the mirror's size is a power of 2
    assert((mirrorSize & (mirrorSize - 1)) == 0);
}

uint8_t AddressableDevice::readFrom(uint16_t addr, bool readOnly) const
{
    uint8_t data = 0x00;
    // Bitwise and performs the address mirroring
    if (addr >= addrBase && addr < addrEnd)
    {
        data = getByte(addr & (mirrorSize - 1), readOnly);
    }

    return data;
}

bool AddressableDevice::writeTo(uint16_t addr, uint8_t data)
{
    bool success = false;
    // Bitwise and performs the address mirroring
    if (addr >= addrBase && addr <= addrEnd)
    {
        setByte(addr & (mirrorSize - 1), data);
        success = true;
    }

    return success;
}