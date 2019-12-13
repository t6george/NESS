#pragma once
#include <Ram.hpp>
#include <GamePak.hpp>

class VRam : public Ram
{
    GamePak::MirrorMode mMode;
    uint16_t mirrorAddress(uint16_t addr, uint16_t mirror) override;

public:
    explicit VRam(const uint16_t size);
    ~VRam() = default;

    void setMirrorType(GamePak::MirrorMode type);
};