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
        uint8_t cyclePenalty = fetchAuxData();
        cpu->A |= auxData;

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);
        return numCycles + cyclePenalty;
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

template <Ricoh2A03::AddressingType T>
class BPL : public AddressingMode<T>
{
public:
    BPL(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (!cpu->getFlag(Ricoh2A03::N))
        {
            cyclePenalty = 1;
            absoluteAddress = cpu->PC + auxData;

            if ((cpu->PC & 0xFF00) != (absoluteAddress & 0xFF00))
                cyclePenalty = 2;
        }

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class CLC : public AddressingMode<T>
{
public:
    CLC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::C, false);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class JSR : public AddressingMode<T>
{
public:
    JSR(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        cpu->pushDoubleWord(cpu->PC - 1);
        cpu->PC = absoluteAddr;
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class AND : public AddressingMode<T>
{
public:
    AND(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();
        cpu->A &= auxData;

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BIT : public AddressingMode<T>
{
public:
    BIT(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();

        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);
        cpu->setFlag(Ricoh2A03::V, auxData & 0x40);

        auxData &= cpu->A;
        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BIT : public AddressingMode<T>
{
public:
    BIT(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();

        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);
        cpu->setFlag(Ricoh2A03::V, auxData & 0x40);

        auxData &= cpu->A;
        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class ROL : public AddressingMode<T>
{
public:
    ROL(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        uint8_t oldCarry = static_cast<uint8_t>(cpu->getFlag(Ricoh2A03::C));
        cpu->setFlag(Ricoh2A03::C, auxData & 0x80);

        auxData = (auxData << 1) | oldCarry;
        writeBack();

        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);
        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PLP : public AddressingMode<T>
{
public:
    PLP(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->S = cpu->popWord() | Ricoh2A03::U;
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BMI : public AddressingMode<T>
{
public:
    BMI(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (cpu->getFlag(Ricoh2A03::N))
        {
            cyclePenalty = 1;
            absoluteAddress = cpu->PC + auxData;

            if ((cpu->PC & 0xFF00) != (absoluteAddress & 0xFF00))
                cyclePenalty = 2;
        }

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class SEC : public AddressingMode<T>
{
public:
    SEC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::C, true);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class RTI : public AddressingMode<T>
{
public:
    RTI(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->S = cpu->popWord();
        cpu->setFlag(Ricoh2A03::U, false);
        cpu->setFlag(Ricoh2A03::B, false);

        cpu->PC = cpu->popDoubleWord();
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class EOR : public AddressingMode<T>
{
public:
    EOR(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();

        cpu->A ^= auxData;

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);
        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class LSR : public AddressingMode<T>
{
public:
    LSR(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();

        cpu->setFlag(Ricoh2A03::C, auxData & 0x01);

        auxData >>= 1;
        writeBack();

        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);
        cpu->setFlag(Ricoh2A03::N, false);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PHA : public AddressingMode<T>
{
public:
    PHA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->pushWord(cpu->A);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class JMP : public AddressingMode<T>
{
public:
    JMP(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        cpu->PC = absoluteAddress;
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BVC : public AddressingMode<T>
{
public:
    BVC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (!cpu->getFlag(Ricoh2A03::V))
        {
            cyclePenalty = 1;
            absoluteAddress = cpu->PC + auxData;

            if ((cpu->PC & 0xFF00) != (absoluteAddress & 0xFF00))
                cyclePenalty = 2;
        }

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class CLI : public AddressingMode<T>
{
public:
    CLI(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::I, false);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class RTS : public AddressingMode<T>
{
public:
    RTS(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->PC = cpu->popDoubleWord();
        ++cpu->PC;
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class ADC : public AddressingMode<T>
{
public:
    ADC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();
        uint32_t overflowCheck = cpu->A + auxData + static_cast<uint8_t>(cpu->getFlag(Ricoh2A03::C));

        cpu->setFlag(Ricoh2A03::C, overflowCheck > 0xFF);
        cpu->setFlag(Ricoh2A03::V,
                     ~(cpu->A ^ auxData) &
                         (cpu->A ^ static_cast<uint8_t>(overflowCheck)) &
                         0x80);

        cpu->A = overflowCheck & 0xFF;
        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class SBC : public AddressingMode<T>
{
public:
    SBC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();

        uint32_t overflowCheck = cpu->A + auxData + static_cast<uint8_t>(cpu->getFlag(Ricoh2A03::C));

        cpu->setFlag(Ricoh2A03::C, overflowCheck > 0xFF);
        cpu->setFlag(Ricoh2A03::V,
                     ~(cpu->A ^ (~auxData)) &
                         (cpu->A ^ static_cast<uint8_t>(overflowCheck)) &
                         0x80);

        cpu->A = overflowCheck & 0xFF;
        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles + cyclePenalty;
    }
};
