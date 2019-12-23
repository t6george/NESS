#pragma once
#include <SDL2/SDL.h>
#include <MOS6502Instruction.hpp>
#include <Ricoh2A03.hpp>

template <Ricoh2A03::AddressingType T>
class AddressingMode : public MOS6502Instruction
{
    const std::string addrModeStr;
protected:
    AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles);

    uint8_t fetchAuxData() override final;

    void writeBack() override final
    {
        if (T == Ricoh2A03::AddressingType::IMP)
            cpu->A = auxData;
        else
            cpu->write(absoluteAddress, auxData);
    }

    inline void instrDump()
    {
        SDL_Log("PC: 0x%04X | OP: %s | AddrMode: %s | Addr: 0x%04X | Fetched Data: 0x%02X\n", 
            oldPC, mnemonic.c_str(), addrModeStr.c_str(), absoluteAddress, auxData);
    }
};