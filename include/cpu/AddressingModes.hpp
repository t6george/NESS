#pragma once
#include <SDL2/SDL.h>
#include <MOS6502Instruction.hpp>
#include <Ricoh2A03.hpp>
#include <iostream>
static unsigned int a = 0;

template <Ricoh2A03::AddressingType T>
class AddressingMode : public MOS6502Instruction
{
    const std::string addrModeStr;

protected:
    AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles);

    uint8_t fetchAuxData(bool loadData = true) override final;

    void writeBack() override final
    {
        if (T == Ricoh2A03::AddressingType::IMP)
            cpu->A = auxData;
        else
            cpu->write(absoluteAddress, auxData);
    }

    inline void instrDump()
    {
        std::cout << a << " - " << std::hex << oldPC << ": " << mnemonic.c_str() << " (" << addrModeStr.c_str() << ")" << std::endl;
        ++a;
        std::cout << "A: " << std::hex << static_cast<int>(cpu->A) << std::endl;
        std::cout << "X: " << std::hex << static_cast<int>(cpu->X) << std::endl;
        std::cout << "Y: " << std::hex << static_cast<int>(cpu->Y) << std::endl;
        std::cout << "SP: " << std::hex << static_cast<int>(cpu->SP) << std::endl;
        std::cout << "S: " << std::hex << static_cast<int>(cpu->S) << std::endl;
        std::cout << "----------------" << std::endl;
        // SDL_Log("%x - PC: 0x%04X | OP: %s | AddrMode: %s | Addr: 0x%04X | Fetched Data: 0x%02X\n",
        //         a++, oldPC, mnemonic.c_str(), addrModeStr.c_str(), absoluteAddress, auxData);
    }

public:
    std::string disassemble(uint16_t &pc) override;
};