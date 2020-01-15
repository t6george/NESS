#include <Apu2A03.hpp>
#include <Ricoh2A03.hpp>
#include <NesSystem.hpp>
#include <stdio.h>
// namespace APU
// {

// void Apu2A03::init()
// {
//     buf.sample_rate(96000);
//     buf.clock_rate(1789773);

//     apu.output(&buf);
//     apu.dmc_reader(func);
// }

Apu2A03::Apu2A03()
{
    apu = new Nes_Apu;
    buf.sample_rate(96000);
    buf.clock_rate(1789773);

    apu->output(&buf);
    apu->dmc_reader(func);
}

void Apu2A03::reset()
{
    // apu->reset();
    // buf.clear();
}

template <bool write>
uint8_t Apu2A03::access(int elapsed, uint16_t addr, uint8_t v)
{
    if (write)
        apu->write_register(elapsed, addr, v);
    else if (addr == apu->status_addr)
        v = apu->read_status(elapsed);

    return v;
}
template uint8_t Apu2A03::access<0>(int, uint16_t, uint8_t);
template uint8_t Apu2A03::access<1>(int, uint16_t, uint8_t);

void Apu2A03::run_frame(int elapsed)
{
    apu->end_frame(elapsed);
    buf.end_frame(elapsed);

    // printf("NES ptr: %p\n", nes);

    if (buf.samples_avail() >= OUT_SIZE)
        nes->newSamples(outBuf, buf.read_samples(outBuf, OUT_SIZE));
}

// } // namespace APU