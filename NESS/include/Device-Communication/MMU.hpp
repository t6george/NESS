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
    // uint8_t controller_state[2];
    // uint8_t controller[2];

public:
    MMU() = default;
    ~MMU() = default;

    void addEntry(const AddressingInfo entry);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};