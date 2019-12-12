#pragma once

#include <Ram.hpp>

class PaletteRam : public Ram
{
public:

    explicit PaletteRam(const uint16_t size);
    ~PaletteRam() = default;
    
    virtual uint8_t read(uint16_t addr, uint16_t mirror, bool readOnly = false) override;
    virtual void write(uint16_t addr, uint16_t mirror, uint8_t data) override;
};