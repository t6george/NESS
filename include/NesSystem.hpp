#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <Display.hpp>
#include <Ricoh2A03.hpp>
#include <Apu2A03.hpp>
#include <Sound_Queue.h>
#include <SDL2/SDL.h>

class RicohRP2C02;
class NesSystem;
class GamePad;

class NesSystem
{
public:
    uint32_t systemClock;
    std::shared_ptr<GamePad> p1Controller;

    std::shared_ptr<RicohRP2C02> ppu;
    std::shared_ptr<Ricoh2A03> cpu;
    std::shared_ptr<Apu2A03> apu;

    std::unique_ptr<Display> screen;
    std::unique_ptr<Sound_Queue> soundQueue;

    uint8_t dma_data;
    bool dma_dummy;

    const uint32_t fps, delay;
    double delayMultiplier;
    uint32_t frameStart, frameTime;
    SDL_Event e;

public:
    NesSystem();
    ~NesSystem() = default;

    void tick();
    void reset();
    void insertCartridge(const std::string &romName);
    uint64_t getFrameCount() const;
    void newSamples(const blip_sample_t *samples, size_t count);
    void processGameplayInput(const SDL_Event &event);
    void outputFrame() const;
    bool run();
};