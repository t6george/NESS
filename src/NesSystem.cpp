#include <iostream>
#include <fstream>

#include <NesSystem.hpp>
#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>
#include <GamePak.hpp>
#include <Display.hpp>
#include <HwConstants.hpp>

NesSystem::NesSystem()
    : ppu{new RicohRP2C02{}}, cpu{new Ricoh2A03{ppu}},
      screen{new Display{DISPLAY::Width, DISPLAY::Height, ppu->getFrameBuffData()}} {}

void NesSystem::tick()
{
    // std::cout << "pc 1 " << std::hex << static_cast<int>(cpu->PC) << std::endl;

    ppu->run();
    // std::cout << "pc 2 " << std::hex << static_cast<int>(cpu->PC) << std::endl;
    if (systemClock % 3 == 0)
    {
        cpu->fetch();
    }

    if (ppu->requestCpuNmi)
    {
        cpu->nmi();
        ppu->requestCpuNmi = false;
    }

    // std::cout << "pc 3 " << std::hex << static_cast<int>(cpu->PC) << std::endl;
    if (ppu->updateScreen)
    {
        screen->blit();
        ppu->updateScreen = false;
    }
    // std::cout << "pc 4 " << std::hex << static_cast<int>(cpu->PC) << std::endl;
    ++systemClock;
}

void NesSystem::reset()
{
    cpu->reset();

    for (uint8_t i = 0; i < 0x15; ++i)
        ppu->run();

    ppu->reset();
    systemClock = 0;
}

#define DISASSEMBLE
void NesSystem::insertCartridge(const std::string &romName)
{
    std::shared_ptr<AddressableDevice> cart(new GamePak(romName));

    cpu->addCartridge(cart);
    ppu->addCartridge(cart);
    reset();

#ifdef DISASSEMBLE
    uint16_t PC = CPU::CARTRIDGE::Base;
    int8_t nameEnd = romName.size() - 1;
    uint8_t opcode;

    while (nameEnd >= 0 && romName[nameEnd--] != '.')
        ;
    ++nameEnd;
    if (nameEnd == 0)
        nameEnd = romName.size() - 1;

    std::ofstream asmFile(romName.substr(0, nameEnd) + ".asm");
    if (asmFile.is_open())
    {
        while (PC >= CPU::CARTRIDGE::Base && PC <= CPU::CARTRIDGE::Limit)
        {
            asmFile << std::hex << PC << " : ";
            opcode = cpu->read(PC++);
            asmFile << cpu->instructions[opcode]->disassemble(PC) << std::endl;
        }

        asmFile.close();
    }
#endif
}