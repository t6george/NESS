#pragma once
#include <vector>
#include <cstdint>

enum class Request
{
    CPU,
    PPU
};

class AddressableDevice
{
protected:
    const uint16_t addrBase;
    const uint16_t addrEnd;
    uint16_t mirrorSize;
    Request requestSrc;

    std::vector<std::vector<uint8_t>> contents;

    virtual void setByte(uint16_t addr, uint8_t data) = 0;
    virtual uint8_t getByte(uint16_t addr, bool readOnly) const = 0;

public:
    AddressableDevice(const uint16_t addrBase, const uint16_t addrEnd);
    AddressableDevice(const std::vector<uint16_t> &sizes, const uint16_t addrBase, const uint16_t addrEnd);

    virtual ~AddressableDevice() = default;
    virtual bool writeTo(uint16_t addr, uint8_t data);
    virtual uint8_t readFrom(uint16_t addr, bool readOnly = false) const;
};