#pragma once
#include <AddressableDevice.hpp>

#include <cstdint>
#include <vector>

enum class Request
{
    CPU,
    PPU
};

class DualAddressableDevice : public AddressableDevice
{
protected:
    const uint16_t secondaryAddrBase;
    const uint16_t secondaryAddrEnd;
    uint16_t secondaryMirrorSize;

    Request src;

public:
    DualAddressableDevice(const std::vector<uint16_t> &sizes, const uint16_t primBase,
                          const uint16_t primEnd, const uint16_t secondBase, const uint16_t secondEnd);
    virtual ~DualAddressableDevice() = default;

    bool writeTo(uint16_t addr, uint8_t data) override;
    uint8_t readFrom(uint16_t addr, bool readOnly = false) const override;
    void setMemorySignalRequester(Request device);
};