#pragma once
#include <vector>
#include <cstdint>

/*
 * NES RAM's address space is 8KB, but it is only 2KB in size
 * This is achieved through 3 additional 2KB regions that mirror
 * the first 2KB region. To achieve this, the modulo of the address
 * and physical RAM size is the actual address being accessed
 */

class AddressableDevice
{
protected:
    const uint16_t addrBase;
    const uint16_t addrEnd;
    std::vector<uint8_t> contents;
    
    virtual void setByte(uint16_t addr, uint8_t data) = 0;
    virtual uint8_t getByte(uint16_t addr, bool readOnly) const = 0;
public:
    AddressableDevice(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd);
    virtual ~AddressableDevice() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false) const;
};