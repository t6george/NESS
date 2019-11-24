#include <Ricoh2A03.hpp>
#include <Bus.hpp>
#include <MOS6502Instruction.hpp>

Ricoh2A03::Ricoh2A03(std::shared_ptr<Bus> bus) : bus{bus} {}

uint8_t Ricoh2A03::read(uint16_t addr)
{
    return bus->read(addr);
    // return 0;
}

void Ricoh2A03::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

void Ricoh2A03::pushWord(uint8_t word)
{
    write(STACK_BASE + SP--, word);
}

void Ricoh2A03::pushDoubleWord(uint16_t dWord)
{
    write(STACK_BASE + SP--, dWord >> 8);
    write(STACK_BASE + SP--, dWord & 0x00FF);
}

uint8_t Ricoh2A03::popWord()
{
    return read(STACK_BASE + SP++);
}

uint16_t Ricoh2A03::popDoubleWord()
{
    uint16_t lo = read(STACK_BASE + SP++);
    uint16_t hi = read(STACK_BASE + SP++);
    return (hi << 8) | lo;
}

void Ricoh2A03::tick()
{
    MOS6502Instruction *currentInstr = nullptr;
    if (cycles == 0)
    {
        (void)currentInstr;
        // opcode = read(PC++);
        // cycles = ;
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

void Ricoh2A03::nmi(uint16_t interruptAddr)
{
    SetFlag(B, false);
    SetFlag(U, true);
    SetFlag(I, true);

    pushDoubleWord(PC);
    pushWord(S);

    PC = (static_cast<uint16_t>(read(interruptAddr + 0x1)) << 8) |
         static_cast<uint16_t>(read(interruptAddr));

    cycles = 8;
}

void Ricoh2A03::irq()
{
    if (GetFlag(I) == 0)
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
    S |= (1 << static_cast<uint8_t>(b));
}