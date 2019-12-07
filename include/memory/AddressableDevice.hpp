#pragma once
#include <cstdint>

class AddressableDevice
{
protected:
    virtual void setByte(uint16_t addr, uint8_t data) = 0;
    virtual uint8_t getByte(uint16_t addr, bool readOnly) const = 0;

public:
    AddressableDevice() = default;
    virtual ~AddressableDevice() = default;

    virtual uint8_t read(uint16_t addr, uint16_t mirror, bool readOnly = false);
    virtual void write(uint16_t addr, uint16_t mirror, uint8_t data);
};