#pragma once
#include <cstdint>
#include <memory>
#include <Nes_Apu.h>

class Ricoh2A03;
class NesSystem;

static const int OUT_SIZE = 4096;

extern void newSamples(const blip_sample_t *samples, size_t count);

class Apu2A03
{
public:
    Apu2A03();
    Nes_Apu apu;
    Blip_Buffer buf;

    blip_sample_t outBuf[OUT_SIZE];

    int (*func)(void *, unsigned int);
    template <bool write>
    uint8_t access(int elapsed, uint16_t addr, uint8_t v = 0);
    void run_frame(int elapsed);
};