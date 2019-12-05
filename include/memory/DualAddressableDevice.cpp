#include <DualAddressableDevice.hpp>

DualAddressableDevice::DualAddressableDevice(const std::vector<uint16_t> &sizes, const uint16_t primBase,
                                             const uint16_t primEnd, const uint16_t secondBase, const uint16_t secondEnd)
    : AddressableDevice{sizes, primBase, primEnd}, secondaryAddrBase{secondaryAddrBase}, secondaryAddrEnd{secondaryAddrEnd} {}

bool DualAddressableDevice::writeTo(uint16_t addr, uint8_t data)
{
}

uint8_t DualAddressableDevice::readFrom(uint16_t addr, bool readOnly = false) const
{
    uint8_t data = 0x00;
    // Bitwise and performs the address mirroring
    if (src == Request::CPU && addr >= addrBase && addr < addrEnd)
    {
        data = getByte(addr & (mirrorSize - 1), readOnly);
    }
    else if (src == Request::PPU && addr >= addrBase && addr < addrEnd)
    {
        data = getByte()
    }

    return data;
}

void DualAddressableDevice::setMemorySignalRequester(Requester device)
{
    src = device;
}