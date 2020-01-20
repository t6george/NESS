#pragma once

#include <Ram.hpp>

class PaletteRam : public Ram
{
    uint16_t mirrorAddress(uint16_t addr, uint16_t mirror);

public:
    explicit PaletteRam(const uint16_t size);
    ~PaletteRam() = default;
};