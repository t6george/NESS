#include <AddressingModes.hpp>
#include <Ricoh2A03.hpp>

/*
 * Undefined opcode
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::NOP>::fetchAuxData()
{
    return 0;
}

template <>
void AddressingMode<Ricoh2A03::AddressingType::NOP>::writeBack()
{
}

/*
 * Instruction is acting on some register, and
 * does not need data from RAM
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IMP>::fetchAuxData()
{
    auxData = cpu->A;

    return 0;
}

template <>
void AddressingMode<Ricoh2A03::AddressingType::IMP>::writeBack()
{
    cpu->A = auxData;
}

/*
 * Instruction has been given an immediate value,
 * so no need to access RAM either
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IMM>::fetchAuxData()
{
    auxData = cpu->read(cpu->PC++);

    return 0;
}

template <>
void AddressingMode<Ricoh2A03::AddressingType::IMP>::writeBack()
{
    cpu->write(cpu->PC - 1, auxData);
}

/*
 * Instruction is given an 8-bit address.
 * The upper byte is implicitly 0x00, so
 * it accesses the RAM's Page 0
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZP>::fetchAuxData()
{
    auxData = cpu->read(cpu->PC++);

    return 0;
}

template <>
void AddressingMode<Ricoh2A03::AddressingType::ZP>::writeBack()
{
    cpu->write(cpu->PC - 1, auxData);
}

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG X
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZPX>::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++)) + cpu->X;

    return 0;
}

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG Y
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZPY>::fetchAuxData()
{
    auxData = cpu->read(cpu->PC++) + cpu->Y;

    return 0;
}

/*
 * Branch offset located at CPU's PC
 * Offset is only 8-bit, so must be sign-extended
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::REL>::fetchAuxData()
{
    auxData = cpu->read(cpu->PC++);
    auxData |= (((0x80 & auxData) != 0x0000) * 0xFF00);

    return 0;
}

/*
 * Given a direct 16-bit address
 * to retrieve the contents of
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::AB>::fetchAuxData()
{
    auxData = (static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
              static_cast<uint16_t>(cpu->read(cpu->PC));
    cpu->PC += 0x2;

    return 0;
}

/*
 * Absolute addressing with an offset of CPU REG X
 * Page cross => 1 cycle penalty
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ABX>::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(cpu->PC))) +
              cpu->X;
    cpu->PC += 0x2;

    return (auxData & 0xFF00) != ((auxData - cpu->X) & 0xFF00);
}

/*
 * Absolute addressing with an offset of CPU REG Y
 * Page cross => 1 cycle penalty
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ABY>::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(cpu->PC))) +
              cpu->Y;
    cpu->PC += 0x2;

    return (auxData & 0xFF00) != ((auxData - cpu->Y) & 0xFF00);
}

/*
 * Address of pointer is encoded in instruction
 * instead of an address of data (like the above
 * addressing modes).
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IN>::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(cpu->PC)));
    cpu->PC += 0x2;

    // Hardware bug: wrap around if low addr byte's bits are all set (no carry is propagated)
    if ((auxData & 0x00FF) == 0x00FF)
    {
        auxData = ((static_cast<uint16_t>(cpu->read(auxData & 0xFF00)) << 8) |
                   static_cast<uint16_t>(cpu->read(auxData)));
    }
    else
    {
        auxData = ((static_cast<uint16_t>(cpu->read(auxData + 0x1)) << 8) |
                   static_cast<uint16_t>(cpu->read(auxData)));
    }

    return 0;
}

/*
 * Indirect addressing, but address of pointer
 * is offset by CPU REG X
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::INX>::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++) + cpu->X);
    auxData = ((static_cast<uint16_t>(cpu->read(auxData + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(auxData)));

    return 0;
}

/*
 * Indirect addressing, but the actual pointer
 * in memory is offset by CPU REG Y right before
 * the data read.
 * Page cross => 1 cycle penalty
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::INY>::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++));
    auxData = ((static_cast<uint16_t>(cpu->read(auxData + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(auxData))) +
              cpu->Y;

    return (auxData & 0xFF00) != ((auxData - cpu->Y) & 0xFF00);
}