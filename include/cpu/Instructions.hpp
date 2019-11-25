#pragma once
#include <cstdint>
#include <AddressingModes.hpp>

class BRK : public AddressingMode<Ricoh2A03::AddressingType::IMP>
{
public:
    BRK(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<Ricoh2A03::AddressingType::IMP>(cpu, numCycles, size) {}

    uint8_t exec() override final
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
};

template <Ricoh2A03::AddressingType T>
class ORA : public AddressingMode<T>
{
public:
    ORA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        return numCycles;
    }
};