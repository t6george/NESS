#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <Display.hpp>
#include <Ricoh2A03.hpp>

class RicohRP2C02;

class NesSystem
{
public:
    uint64_t frameCount;
    uint32_t systemClock;
    std::shared_ptr<RicohRP2C02> ppu;
    std::shared_ptr<Ricoh2A03> cpu;
    std::unique_ptr<Display> screen;

public:
    NesSystem();
    ~NesSystem() = default;

    void tick();
    void reset();
    void insertCartridge(const std::string &romName);
    uint64_t getFrameCount() const;
};