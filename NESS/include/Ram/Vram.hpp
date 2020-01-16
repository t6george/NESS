#pragma once
#include <Ram.hpp>
#include <GamePak.hpp>

class VRam : public Ram
{
    GamePak::MirrorMode mMode;
    uint16_t mirrorAddress(uint16_t addr, uint16_t mirror) override;

public:
    VRam(const uint16_t size, GamePak::MirrorMode mMode);
    ~VRam() = default;
};