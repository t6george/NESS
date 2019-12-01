#pragma once
#include <AddressableDevice.hpp>

/*
 * NES RAM's address space is 8KB, but it is only 2KB in size
 * This is achieved through 3 additional 2KB regions that mirror
 * the first 2KB region. To achieve this, the modulo of the address
 * and physical RAM size is the actual address being accessed
 */

class Ram : public AddressableDevice
{
protected:
    virtual void setByte(uint16_t addr, uint8_t data) override;
    virtual uint8_t getByte(uint16_t addr, bool readOnly) const override;
public:
    Ram(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd);
};