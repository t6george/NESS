#pragma once
#include <Ram.hpp>

class VRam : public Ram
{
public:

    explicit VRam(const uint16_t size);
    ~VRam() = default;
    
    virtual uint8_t read(uint16_t addr, uint16_t mirror, bool readOnly = false) override;
    virtual void write(uint16_t addr, uint16_t mirror, uint8_t data) override;
};