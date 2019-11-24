#pragma once
#include <MOS6502Instruction.hpp>

namespace Addressing
{
/*
 * Instruction is acting on some register, and 
 * does not need data from RAM
 */
class Implied : public MOS6502Instruction
{
protected:
    Implied(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Instruction has been given an immediate value,
 * so no need to access RAM either
 */
class Immediate : public MOS6502Instruction
{
protected:
    Immediate(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Instruction is given an 8-bit address.
 * The upper byte is implicitly 0x00, so
 * it accesses the RAM's Page 0
 */
class ZPage : public MOS6502Instruction
{
protected:
    ZPage(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG X
 */
class ZXPage : public MOS6502Instruction
{
protected:
    ZXPage(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG Y
 */
class ZYPage : public MOS6502Instruction
{
protected:
    ZYPage(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Branch offset located at CPU's PC
 * Offset is only 8-bit, so must be sign-extended
 */
class Relative : public MOS6502Instruction
{
protected:
    Relative(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Given a direct 16-bit address
 * to retrieve the contents of
 */
class Absolute : public MOS6502Instruction
{
protected:
    Absolute(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Absolute addressing with an offset of CPU REG X
 * Page cross => 1 cycle penalty
 */
class AbsoluteX : public MOS6502Instruction
{
protected:
    AbsoluteX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Absolute addressing with an offset of CPU REG Y
 * Page cross => 1 cycle penalty
 */
class AbsoluteY : public MOS6502Instruction
{
protected:
    AbsoluteY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Address of pointer is encoded in instruction
 * instead of an address of data (like the above
 * addressing modes). 
 */
class Indirect : public MOS6502Instruction
{
protected:
    Indirect(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Indirect addressing, but address of pointer
 * is offset by CPU REG X
 */
class IndirectZX : public MOS6502Instruction
{
protected:
    IndirectZX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};

/*
 * Indirect addressing, but the actual pointer
 * in memory is offset by CPU REG Y right before
 * the data read.
 * Page cross => 1 cycle penalty
 */
class IndirectZY : public MOS6502Instruction
{
protected:
    IndirectZY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t fetchAuxData() override final;
};
} // namespace Addressing