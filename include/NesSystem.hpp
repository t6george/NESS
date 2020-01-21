#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <Display.hpp>
#include <Ricoh2A03.hpp>
#include <Apu2A03.hpp>
#include <Sound_Queue.h>

class RicohRP2C02;
class NesSystem;

static std::shared_ptr<NesSystem> nes;

class NesSystem
{
public:
    uint32_t systemClock;

    std::shared_ptr<RicohRP2C02> ppu;
    std::shared_ptr<Ricoh2A03> cpu;
    std::shared_ptr<Apu2A03> apu;

    std::unique_ptr<Display> screen;
    Sound_Queue *soundQueue;

    uint8_t dma_data = 0x00;

    bool dma_dummy = true;

public:
    NesSystem();
    ~NesSystem() = default;

    void tick();
    void reset();
    void insertCartridge(const std::string &romName);
    uint64_t getFrameCount() const;
    void newSamples(const blip_sample_t *samples, size_t count);
};