#include <Instructions.hpp>
#include <Ricoh2A03.hpp>

BRK::BRK(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : Addressing::Implied(cpu, numCycles, size) {}

uint8_t BRK::exec()
{
    ++cpu->PC;
    cpu->SetFlag(Ricoh2A03::I, true);
    cpu->SetFlag(Ricoh2A03::B, true);

    cpu->pushDoubleWord(cpu->PC);
    cpu->pushWord(cpu->S);

    cpu->PC = (static_cast<uint16_t>(cpu->read(0xFFFF)) << 8) | static_cast<uint16_t>(cpu->read(0xFFFE));
    cpu->SetFlag(Ricoh2A03::B, false);

    return numCycles;
}

ORA::BRK(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : Addressing::Implied(cpu, numCycles, size) {}

uint8_t BRK::exec()
{
    ++cpu->PC;
    cpu->SetFlag(Ricoh2A03::I, true);
    cpu->SetFlag(Ricoh2A03::B, true);

    cpu->pushDoubleWord(cpu->PC);
    cpu->pushWord(cpu->S);

    cpu->PC = (static_cast<uint16_t>(cpu->read(0xFFFF)) << 8) | static_cast<uint16_t>(cpu->read(0xFFFE));
    cpu->SetFlag(Ricoh2A03::B, false);

    return numCycles;
}