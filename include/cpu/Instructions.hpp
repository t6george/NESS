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
class ROR : public AddressingMode<T>
{
public:
    ROR(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        uint8_t oldCarry = static_cast<uint8_t>(cpu->getFlag(Ricoh2A03::C)) << 7;
        cpu->setFlag(Ricoh2A03::C, auxData & 0x01);

        auxData = (auxData >> 1) | oldCarry;
        writeBack();

        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);
        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PLA : public AddressingMode<T>
{
public:
    PLA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->A = cpu->popWord();

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BVS : public AddressingMode<T>
{
public:
    BVS(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (cpu->getFlag(Ricoh2A03::V))
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
class SEI : public AddressingMode<T>
{
public:
    SEI(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::I, true);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class STA : public AddressingMode<T>
{
public:
    STA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        auxData = cpu->A;
        writeBack();
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class STY : public AddressingMode<T>
{
public:
    STY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        auxData = cpu->Y;
        writeBack();
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class STX : public AddressingMode<T>
{
public:
    STX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        auxData = cpu->X;
        writeBack();
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class DEY : public AddressingMode<T>
{
public:
    DEY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        --cpu->Y;

        cpu->setFlag(Ricoh2A03::Z, cpu->Y == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->Y & 0x80);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TXA : public AddressingMode<T>
{
public:
    TXA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->A = cpu->X;

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BCC : public AddressingMode<T>
{
public:
    BCC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (!cpu->getFlag(Ricoh2A03::C))
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
class TYA : public AddressingMode<T>
{
public:
    TYA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->A = cpu->Y;

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TXS : public AddressingMode<T>
{
public:
    TXS(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->SP = cpu->X;
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class LDY : public AddressingMode<T>
{
public:
    LDY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();
        cpu->Y = auxData;

        cpu->setFlag(Ricoh2A03::Z, cpu->Y == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->Y & 0x80);

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class LDA : public AddressingMode<T>
{
public:
    LDA(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();
        cpu->A = auxData;

        cpu->setFlag(Ricoh2A03::Z, cpu->A == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->A & 0x80);

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class LDX : public AddressingMode<T>
{
public:
    LDX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();
        cpu->X = auxData;

        cpu->setFlag(Ricoh2A03::Z, cpu->X == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->X & 0x80);

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class TAY : public AddressingMode<T>
{
public:
    TAY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->Y = cpu->A;

        cpu->setFlag(Ricoh2A03::Z, cpu->Y == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->Y & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TAX : public AddressingMode<T>
{
public:
    TAX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->X = cpu->A;

        cpu->setFlag(Ricoh2A03::Z, cpu->X == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->X & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BCS : public AddressingMode<T>
{
public:
    BCS(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (cpu->getFlag(Ricoh2A03::C))
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
class CLV : public AddressingMode<T>
{
public:
    CLV(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::V, false);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TSX : public AddressingMode<T>
{
public:
    TSX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->X = cpu->SP;

        cpu->setFlag(Ricoh2A03::Z, cpu->X == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->X & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CPY : public AddressingMode<T>
{
public:
    CPY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        uint8_t result = cpu->Y - auxData;

        cpu->setFlag(Ricoh2A03::Z, result == 0x00);
        cpu->setFlag(Ricoh2A03::N, result & 0x80);
        cpu->setFlag(Ricoh2A03::C, cpu->Y >= auxData);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CMP : public AddressingMode<T>
{
public:
    CMP(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = fetchAuxData();
        uint8_t result = cpu->A - auxData;

        cpu->setFlag(Ricoh2A03::Z, result == 0x00);
        cpu->setFlag(Ricoh2A03::N, result & 0x80);
        cpu->setFlag(Ricoh2A03::C, cpu->A >= auxData);

        return numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class DEC : public AddressingMode<T>
{
public:
    DEC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        --auxData;

        writeBack();
        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);
        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class INY : public AddressingMode<T>
{
public:
    INY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        ++cpu->Y;
        cpu->setFlag(Ricoh2A03::Z, cpu->Y == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->Y & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class DEX : public AddressingMode<T>
{
public:
    DEX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        --cpu->X;
        cpu->setFlag(Ricoh2A03::Z, cpu->X == 0x00);
        cpu->setFlag(Ricoh2A03::N, cpu->X & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BNE : public AddressingMode<T>
{
public:
    BNE(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (!cpu->getFlag(Ricoh2A03::Z))
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
class CLD : public AddressingMode<T>
{
public:
    CLD(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        cpu->setFlag(Ricoh2A03::D, false);
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CPX : public AddressingMode<T>
{
public:
    CPX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        uint8_t result = cpu->X - auxData;

        cpu->setFlag(Ricoh2A03::Z, result == 0x00);
        cpu->setFlag(Ricoh2A03::N, result & 0x80);
        cpu->setFlag(Ricoh2A03::C, cpu->X >= auxData);

        return numCycles;
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

template <Ricoh2A03::AddressingType T>
class INC : public AddressingMode<T>
{
public:
    INC(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        fetchAuxData();
        ++auxData;

        writeBack();
        cpu->setFlag(Ricoh2A03::Z, auxData == 0x00);
        cpu->setFlag(Ricoh2A03::N, auxData & 0x80);

        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class NOP : public AddressingMode<T>
{
public:
    NOP(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        return numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class BEQ : public AddressingMode<T>
{
public:
    BEQ(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : AddressingMode<T>(cpu, numCycles, size) {}

    uint8_t exec() override final
    {
        int cyclePenalty = 0;
        fetchData();
        if (cpu->getFlag(Ricoh2A03::Z))
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
        cpu->setFlag(Ricoh2A03::D, true);
        return numCycles;
    }
};
