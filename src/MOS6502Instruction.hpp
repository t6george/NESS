#pragma once
#include <cstdint>
#include <string>
#include <memory>

class Ricoh2A03;

class MOS6502Instruction
{
    shared_ptr<Ricoh2A03> cpu;
    uint16_t auxData;
    uint8_t numCycles;
    uint8_t size;

    public:

    std::string mnemonic;

    MOS6502Instruction(std::string mnemonic, uint8_t numCycles, uint8_t size);
    virtual ~MOS6502Instruction() = default;

    virtual uint8_t fetchAuxData() = 0;
    virtual uint8_t exec() = 0;
};