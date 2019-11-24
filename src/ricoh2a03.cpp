#include "ricoh2a03.hpp"

Ricoh2A03::Ricoh2A03(shared_ptr<Bus> bus): bus{bus}
{
}

~Ricoh2A03()::Ricoh2A03()
{
}

uint8_t Ricoh2A03::read(uint16_t addr)
{
    bus->read(addr);
}

void Ricoh2A03::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

void Ricoh2A03::tick()
{
    if(cycles == 0)
    {
        opcode = read(PC++);
        cycles = ;
    }
    --cycles;
}

void Ricoh2A03::reset()
{
    A = X = Y = 0;
    SP = 0xFD;
    S = 0x0 | U;

    uint16_t lo = read(0xFFFC);
    uint16_t hi = read(0xFFFD);

    PC = (hi << 8) | lo;

    cycles = 8;
}

void Ricoh2A03::nmi(uint16_t interruptAddr = 0xFFFA)
    SetFlag(B, false);
    SetFlag(U, true);
    SetFlag(I, true);

    write(0x0100 + SP--, PC >> 8);
    write(0x0100 + SP--, PC & 0x00FF);
    write(0x0100 + SP--, S);

    uint16_t lo = read(interruptAddr);
    uint16_t hi = read(interruptAddr);

    PC = (hi << 8) | lo;
    cycles = 8;

}

void Ricoh2A03::irq()
{
    if(GetFlag(I) == 0)
    {
        nmi(0xFFFE);
        --cycles;
    }
}

bool Ricoh2A03::GetFlag(Flags6502 f) const
{
    return static_cast<bool>(S & (1 << f));
}

void Ricoh2A03::SetFlag(Flags6502 f, bool b)
{
    S &= ~(1 << f);
    S |= (1 << static_cast<uint8_t>(b))
}