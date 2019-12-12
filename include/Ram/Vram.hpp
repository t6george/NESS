#pragma once
#include <Ram.hpp>

class VRam : public Ram
{
    uint16_t mirrorAddress(uint16_t addr, uint16_t mirror) override;

public:
    explicit VRam(const uint16_t size);
    ~VRam() = default;
};