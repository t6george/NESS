#pragma once
#include <cstdint>
#include <vector>
#include <memory>

class AddressableDevice;

typedef struct
{
    uint16_t base;
    uint16_t limit;
    uint16_t mirror;
    std::shared_ptr<AddressableDevice> device;
} AddressingInfo;

class MMU
{
    std::vector<AddressingInfo> virtToPhys;

public:
    MMU(const std::vector<AddressingInfo> &table);
    ~MMU() = default;

    void addEntry(const AddressingInfo entry);
    uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t data) const;
};