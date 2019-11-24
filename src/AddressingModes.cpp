#include <AddressingModes.hpp>

uint8_t Addressing::Implied::fetchAuxData()
{
    auxData = cpu->A;
    return 0;
}

uint8_t Addressing::Immmediate::fetchAuxData()
{
    auxData = cpu->PC++;
    return 0;
}

uint8_t Addressing::ZPage::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++));
    return 0;
}

uint8_t Addressing::ZXPage::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++)) + cpu->X;
    return 0;
}

uint8_t Addressing::ZYPage::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++)) + cpu->Y;
    return 0;
}

uint8_t Addressing::Relative::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++));
    auxData |= ((0x80 & auxData != 0x0000) * 0xFF00);
    return 0;
}

uint8_t Addressing::Absolute::fetchAuxData()
{
    auxData = (static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) | 
        static_cast<uint16_t>(cpu->read(cpu->PC));
    cpu->PC += 0x2;
    return 0;
}

uint8_t Addressing::AbsoluteX::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) | 
        static_cast<uint16_t>(cpu->read(cpu->PC))) + cpu->X;
    cpu->PC += 0x2;
    return auxData & 0xFF00 != (auxData - cpu->X) & 0xFF00;
}

uint8_t Addressing::AbsoluteY::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) | 
        static_cast<uint16_t>(cpu->read(cpu->PC))) + cpu->Y;
    cpu->PC += 0x2;
    return auxData & 0xFF00 != (auxData - cpu->Y) & 0xFF00;
}

uint8_t Addressing::Indirect::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) | 
        static_cast<uint16_t>(cpu->read(cpu->PC)));
    cpu->PC += 0x2;

    // Hardware bug: wrap around if low addr byte's bits are all set (no carry is propagated)
    if(auxData & 0x00FF == 0x00FF)
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