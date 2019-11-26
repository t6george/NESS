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
        cpu->setFlag(Ricoh2A03::I, true);
        cpu->setFlag(Ricoh2A03::B, true);

        cpu->pushDoubleWord(cpu->PC);
        cpu->pushWord(cpu->S);

        cpu->PC = (static_cast<uint16_t>(cpu->read(0xFFFF)) << 8) | static_cast<uint16_t>(cpu->read(0xFFFE));
        cpu->setFlag(Ricoh2A03::B, false);

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
        fetchAuxData();
        cpu->A |= auxData;
        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class ASL : public AddressingMode<T>
{
public:
    ASL(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        cpu->setFlag(Ricoh2A03::C, auxData & 0x80);

        auxData <<= 1;
        writeBack();

        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);
        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PHP : public AddressingMode<T>
{
public:
    PHP(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::U, true);
        cpu->setFlag(Ricoh2A03::B, true);

        cpu->pushWord(cpu->S);

        cpu->setFlag(Ricoh2A03::U, false);
        cpu->setFlag(Ricoh2A03::B, false);

        return numCycles;
    }
};