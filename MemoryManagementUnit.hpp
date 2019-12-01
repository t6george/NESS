#pragma once
#include <vector>
#include <utility>

enum class Addressable
{
    RAM,
    PPU,
    CART,
};

class MemoryManagementUnit
{
    std::vector<std::pair<uint16_t, uint16_t> memoryMapping;
    public:
    MemoryManagementUnit();
    ~MemoryManagementUnit();
};