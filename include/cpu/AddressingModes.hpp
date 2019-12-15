#pragma once
#include <MOS6502Instruction.hpp>
#include <Ricoh2A03.hpp>

template <Ricoh2A03::AddressingType T>
class AddressingMode : public MOS6502Instruction
{
protected:
    AddressingMode(Ricoh2A03 *cpu, uint8_t numCycles)
     : MOS6502Instruction(cpu, numCycles){};

    uint8_t fetchAuxData() override final;

    void writeBack() override final
    {
        if (T == Ricoh2A03::AddressingType::IMP)
            cpu->A = auxData;
        else
            cpu->write(absoluteAddress, auxData);
    }
};