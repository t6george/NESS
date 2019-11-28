#include <Ricoh2A03.hpp>
#include <Bus.hpp>
#include <Instructions.hpp>

#define GEN_INSTR(name, templ, cycles, size) std::unique_ptr<MOS6502Instruction>(new name templ(this, (cycles), (size)))

Ricoh2A03::Ricoh2A03(std::shared_ptr<Bus> bus) : bus{bus}, instructions{GEN_INSTR(RTI, <Ricoh2A03::AddressingType::IMP>, 0, 2)} //instructions{std::unique_ptr<MOS6502Instruction>(new BRK(nullptr, 1, 2))} //instructions{std::unique_ptr<MOS6502Instruction>(new RTI<Ricoh2A03::AddressingType::IMP>(nullptr, 1, 2))}
{
    // instructions[0] = new RTI<Ricoh2A03::AddressingType::IMP>(nullptr, 1, 2);
}

uint8_t Ricoh2A03::read(uint16_t addr)
{
    return bus->read(addr);
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
    S = U;

    PC = (static_cast<uint16_t>(read(0xFFFD)) << 8) |
         static_cast<uint16_t>(read(0xFFFC));

    cycles = 8;
}

void Ricoh2A03::nmi(uint16_t interruptAddr)
{
    setFlag(U, true);
    setFlag(I, true);
    setFlag(B, false);

    pushDoubleWord(PC);
    pushWord(S);

    PC = (static_cast<uint16_t>(read(interruptAddr + 0x1)) << 8) |
         static_cast<uint16_t>(read(interruptAddr));

    cycles = 8;
}

void Ricoh2A03::irq()
{
    if (!getFlag(I))
    {
        nmi(0xFFFE);
        --cycles;
    }
}

bool Ricoh2A03::getFlag(Flags6502 f) const
{
    return static_cast<bool>(S & f);
}

void Ricoh2A03::setFlag(Flags6502 f, bool b)
{
    S &= ~(1 << f);
    S |= (1 << static_cast<uint8_t>(b));
}

uint8_t Ricoh2A03::branch(uint8_t auxData, uint16_t absoluteAddress, bool cond)
{
    uint8_t cyclePenalty = 0;

    if (cond)
    {
        cyclePenalty = 1;
        absoluteAddress = PC + auxData;

        if ((PC & 0xFF00) != (absoluteAddress & 0xFF00))
            cyclePenalty = 2;
    }

    return cyclePenalty;
}