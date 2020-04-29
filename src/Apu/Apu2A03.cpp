#include <Apu2A03.hpp>

Apu2A03::Apu2A03()
{
    buf.sample_rate(96000);
    buf.clock_rate(1789773);

    apu.output(&buf);
    apu.dmc_reader(func);
}

template <bool write>
uint8_t Apu2A03::access(int elapsed, uint16_t addr, uint8_t v)
{
    if (write)
        apu.write_register(elapsed, addr, v);
    else if (addr == apu.status_addr)
        v = apu.read_status(elapsed);

    return v;
}
template uint8_t Apu2A03::access<0>(int, uint16_t, uint8_t);
template uint8_t Apu2A03::access<1>(int, uint16_t, uint8_t);

void Apu2A03::run_frame(int elapsed)
{
    apu.end_frame(elapsed);
    buf.end_frame(elapsed);

    if (buf.samples_avail() >= OUT_SIZE)
        newSamples(outBuf, buf.read_samples(outBuf, OUT_SIZE));
}