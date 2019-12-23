#pragma once
#include <cstdint>
#include <string>
#include <memory>

class Ricoh2A03;
/*
 * Instruction Abstract Base Class
 * I view an instruction as its own entity
 * that modifies the state of the system
 * This allowed me to make a nice inheritance 
 * hierarchy for all of the instructions.
 * Although jumping through vtables may be
 * slightly less efficient at runtime, the 
 * MOS6502 is 45 years old and can be emulated
 * on the vast majority of modern processors
 * with plenty of resources to spare.
*/
class MOS6502Instruction
{
protected:
    /*
     * Instruction mnemonic used for debugging and disassembly
    */
    const std::string mnemonic;

    /*
     * The PC before it is incremented by the instruction - for disassembly
    */
    uint16_t oldPC;

    /*
     * MOS6502 reference used to change cpu state
    */
    Ricoh2A03* cpu;

    /* 
     * The additional data the instruction would read
     * from memory would get stored here
    */
    uint8_t auxData;

    /* 
     * The address of the byte accessed by the instruction.
     * Used to write back values to ram
    */
    uint16_t absoluteAddress;

    /*
     * Number of cycles for instruction to complete
     * Note that this would be the base number, and the
     * additional clock cycles due to taking a branch
     * or crossing a page boundary will be computed and
     * added by the exec() method at runtime (as appropriate)
    */
    uint8_t numCycles;

    MOS6502Instruction(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
     : mnemonic{mnemonic}, cpu{cpu}, auxData{0x0000}, numCycles{numCycles} {}

    // Fetches any data from RAM that the instruction will execute on
    virtual uint8_t fetchAuxData() = 0;

    // Writes back to location where auxiliary data was read from
    virtual void writeBack() = 0;

public:
    virtual ~MOS6502Instruction() = default;

    // The instruction's actual functionality
    virtual uint8_t exec() = 0;
    virtual std::string disassemble(uint16_t &pc) = 0;
};