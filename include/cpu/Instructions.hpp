#pragma once
#include <cstdint>
#include <AddressingModes.hpp>

// Interrupt Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class BRK : public AddressingMode<T>
{
public:
    BRK(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        ++this->cpu->PC;
        this->cpu->setFlag(Ricoh2A03::I, true);
        this->cpu->setFlag(Ricoh2A03::B, true);

        this->cpu->pushDoubleWord(this->cpu->PC);
        this->cpu->pushWord(this->cpu->S);

        this->cpu->PC = (static_cast<uint16_t>(this->cpu->read(0xFFFF)) << 8) |
                        static_cast<uint16_t>(this->cpu->read(0xFFFE));
        this->cpu->setFlag(Ricoh2A03::B, false);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class RTI : public AddressingMode<T>
{
public:
    RTI(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->S = this->cpu->popWord();
        this->cpu->setFlag(Ricoh2A03::U, false);
        this->cpu->setFlag(Ricoh2A03::B, false);

        this->cpu->PC = this->cpu->popDoubleWord();
        return this->numCycles;
    }
};

// Subroutine Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class JSR : public AddressingMode<T>
{
public:
    JSR(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        this->cpu->pushDoubleWord(this->cpu->PC - 1);
        this->cpu->PC = this->absoluteAddress;
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class RTS : public AddressingMode<T>
{
public:
    RTS(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->PC = this->cpu->popDoubleWord();
        ++this->cpu->PC;
        return this->numCycles;
    }
};

// Direct Stack Intructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class PLA : public AddressingMode<T>
{
public:
    PLA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->A = this->cpu->popWord();

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PHA : public AddressingMode<T>
{
public:
    PHA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->pushWord(this->cpu->A);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PLP : public AddressingMode<T>
{
public:
    PLP(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->S = this->cpu->popWord() | Ricoh2A03::U;
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class PHP : public AddressingMode<T>
{
public:
    PHP(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::U, true);
        this->cpu->setFlag(Ricoh2A03::B, true);

        this->cpu->pushWord(this->cpu->S);

        this->cpu->setFlag(Ricoh2A03::U, false);
        this->cpu->setFlag(Ricoh2A03::B, false);

        return this->numCycles;
    }
};

// Load Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class LDY : public AddressingMode<T>
{
public:
    LDY(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        this->cpu->Y = this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->Y == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->Y & 0x80);

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class LDA : public AddressingMode<T>
{
public:
    LDA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        this->cpu->A = this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class LDX : public AddressingMode<T>
{
public:
    LDX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        this->cpu->X = this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->X == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->X & 0x80);

        return this->numCycles + cyclePenalty;
    }
};

// Store Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class STA : public AddressingMode<T>
{
public:
    STA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        this->auxData = this->cpu->A;
        this->writeBack();
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class STY : public AddressingMode<T>
{
public:
    STY(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        this->auxData = this->cpu->Y;
        this->writeBack();
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class STX : public AddressingMode<T>
{
public:
    STX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        this->auxData = this->cpu->X;
        this->writeBack();
        return this->numCycles;
    }
};

// Register Transfer Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class TAX : public AddressingMode<T>
{
public:
    TAX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->X = this->cpu->A;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->X == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->X & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TSX : public AddressingMode<T>
{
public:
    TSX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->X = this->cpu->SP;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->X == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->X & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TAY : public AddressingMode<T>
{
public:
    TAY(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->Y = this->cpu->A;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->Y == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->Y & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TYA : public AddressingMode<T>
{
public:
    TYA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->A = this->cpu->Y;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TXS : public AddressingMode<T>
{
public:
    TXS(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->SP = this->cpu->X;
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class TXA : public AddressingMode<T>
{
public:
    TXA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->A = this->cpu->X;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles;
    }
};

// Branch Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class BEQ : public AddressingMode<T>
{
public:
    BEQ(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             this->cpu->getFlag(Ricoh2A03::Z));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BNE : public AddressingMode<T>
{
public:
    BNE(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             !this->cpu->getFlag(Ricoh2A03::Z));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BCS : public AddressingMode<T>
{
public:
    BCS(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             this->cpu->getFlag(Ricoh2A03::C));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BCC : public AddressingMode<T>
{
public:
    BCC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             !this->cpu->getFlag(Ricoh2A03::C));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BVS : public AddressingMode<T>
{
public:
    BVS(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             this->cpu->getFlag(Ricoh2A03::V));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BVC : public AddressingMode<T>
{
public:
    BVC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             !this->cpu->getFlag(Ricoh2A03::V));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BPL : public AddressingMode<T>
{
public:
    BPL(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             !this->cpu->getFlag(Ricoh2A03::N));

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BMI : public AddressingMode<T>
{
public:
    BMI(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        int cyclePenalty = this->cpu->branch(this->absoluteAddress,
                                             this->cpu->getFlag(Ricoh2A03::N));

        return this->numCycles + cyclePenalty;
    }
};

// Increment/Decrement Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class INC : public AddressingMode<T>
{
public:
    INC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        ++this->auxData;

        this->writeBack();
        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->auxData & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class INX : public AddressingMode<T>
{
public:
    INX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        ++this->cpu->X;
        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->X == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->X & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class INY : public AddressingMode<T>
{
public:
    INY(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        ++this->cpu->Y;
        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->Y == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->Y & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class DEX : public AddressingMode<T>
{
public:
    DEX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        --this->cpu->X;
        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->X == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->X & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class DEC : public AddressingMode<T>
{
public:
    DEC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        --this->auxData;

        this->writeBack();
        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->auxData & 0x80);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class DEY : public AddressingMode<T>
{
public:
    DEY(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        --this->cpu->Y;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->Y == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->Y & 0x80);
        return this->numCycles;
    }
};

// Compare Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class CPX : public AddressingMode<T>
{
public:
    CPX(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        uint8_t result = this->cpu->X - this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, result == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, result & 0x80);
        this->cpu->setFlag(Ricoh2A03::C, this->cpu->X >= this->auxData);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CPY : public AddressingMode<T>
{
public:
    CPY(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        uint8_t result = this->cpu->Y - this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, result == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, result & 0x80);
        this->cpu->setFlag(Ricoh2A03::C, this->cpu->Y >= this->auxData);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CMP : public AddressingMode<T>
{
public:
    CMP(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        uint8_t result = this->cpu->A - this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, result == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, result & 0x80);
        this->cpu->setFlag(Ricoh2A03::C, this->cpu->A >= this->auxData);

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class BIT : public AddressingMode<T>
{
public:
    BIT(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();

        this->cpu->setFlag(Ricoh2A03::N, this->auxData & 0x80);
        this->cpu->setFlag(Ricoh2A03::V, this->auxData & 0x40);

        this->auxData &= this->cpu->A;
        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);

        return this->numCycles;
    }
};

// Set/Reset Flag Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class SEC : public AddressingMode<T>
{
public:
    SEC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::D, true);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CLD : public AddressingMode<T>
{
public:
    CLD(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::D, false);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class SED : public AddressingMode<T>
{
public:
    SED(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::D, true);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CLV : public AddressingMode<T>
{
public:
    CLV(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::V, false);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class SEI : public AddressingMode<T>
{
public:
    SEI(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::I, true);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CLC : public AddressingMode<T>
{
public:
    CLC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::C, false);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class CLI : public AddressingMode<T>
{
public:
    CLI(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->cpu->setFlag(Ricoh2A03::I, false);
        return this->numCycles;
    }
};

// Bitwise Logic Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class ORA : public AddressingMode<T>
{
public:
    ORA(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        this->cpu->A |= this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);
        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class ASL : public AddressingMode<T>
{
public:
    ASL(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        this->cpu->setFlag(Ricoh2A03::C, this->auxData & 0x80);

        this->auxData <<= 1;
        this->writeBack();

        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->auxData & 0x80);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class AND : public AddressingMode<T>
{
public:
    AND(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        this->cpu->A &= this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class EOR : public AddressingMode<T>
{
public:
    EOR(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();

        this->cpu->A ^= this->auxData;

        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);
        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class LSR : public AddressingMode<T>
{
public:
    LSR(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();

        this->cpu->setFlag(Ricoh2A03::C, this->auxData & 0x01);

        this->auxData >>= 1;
        this->writeBack();

        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, false);
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class ROR : public AddressingMode<T>
{
public:
    ROR(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        uint8_t oldCarry = static_cast<uint8_t>(this->cpu->getFlag(Ricoh2A03::C)) << 7;
        this->cpu->setFlag(Ricoh2A03::C, this->auxData & 0x01);

        this->auxData = (this->auxData >> 1) | oldCarry;
        this->writeBack();

        this->cpu->setFlag(Ricoh2A03::N, this->auxData & 0x80);
        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);

        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class ROL : public AddressingMode<T>
{
public:
    ROL(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        uint8_t oldCarry = static_cast<uint8_t>(this->cpu->getFlag(Ricoh2A03::C));
        this->cpu->setFlag(Ricoh2A03::C, this->auxData & 0x80);

        this->auxData = (this->auxData << 1) | oldCarry;
        this->writeBack();

        this->cpu->setFlag(Ricoh2A03::N, this->auxData & 0x80);
        this->cpu->setFlag(Ricoh2A03::Z, this->auxData == 0x00);

        return this->numCycles;
    }
};

// Arithmetic Instructions ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class ADC : public AddressingMode<T>
{
public:
    ADC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();
        uint32_t overflowCheck = this->cpu->A + this->auxData +
                                 static_cast<uint8_t>(this->cpu->getFlag(Ricoh2A03::C));

        this->cpu->setFlag(Ricoh2A03::C, overflowCheck > 0xFF);
        this->cpu->setFlag(Ricoh2A03::V,
                           ~(this->cpu->A ^ this->auxData) &
                               (this->cpu->A ^ static_cast<uint8_t>(overflowCheck)) &
                               0x80);

        this->cpu->A = overflowCheck & 0xFF;
        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles + cyclePenalty;
    }
};

template <Ricoh2A03::AddressingType T>
class SBC : public AddressingMode<T>
{
public:
    SBC(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        uint8_t cyclePenalty = this->fetchAuxData();

        uint32_t overflowCheck = this->cpu->A + this->auxData +
                                 static_cast<uint8_t>(this->cpu->getFlag(Ricoh2A03::C));

        this->cpu->setFlag(Ricoh2A03::C, overflowCheck > 0xFF);
        this->cpu->setFlag(Ricoh2A03::V,
                           ~(this->cpu->A ^ (~this->auxData)) &
                               (this->cpu->A ^ static_cast<uint8_t>(overflowCheck)) &
                               0x80);

        this->cpu->A = overflowCheck & 0xFF;
        this->cpu->setFlag(Ricoh2A03::Z, this->cpu->A == 0x00);
        this->cpu->setFlag(Ricoh2A03::N, this->cpu->A & 0x80);

        return this->numCycles + cyclePenalty;
    }
};

// MISC ------------------------------------------------------
template <Ricoh2A03::AddressingType T>
class JMP : public AddressingMode<T>
{
public:
    JMP(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        this->fetchAuxData();
        this->cpu->PC = this->absoluteAddress;
        return this->numCycles;
    }
};

template <Ricoh2A03::AddressingType T>
class NOP : public AddressingMode<T>
{
public:
    NOP(Ricoh2A03 *cpu, uint8_t numCycles) : AddressingMode<T>(cpu, numCycles) {}

    uint8_t exec() override final
    {
        return this->numCycles;
    }
};