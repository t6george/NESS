#include <fstream>

#include <NesSystem.hpp>
#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>
#include <GamePak.hpp>
#include <Display.hpp>
#include <HwConstants.hpp>
#include <Apu2A03.hpp>
#include <GamePad.hpp>

NesSystem::NesSystem()
    : systemClock{0}, p1Controller{new GamePad{}}, ppu{new RicohRP2C02{}}, cpu{new Ricoh2A03{ppu, p1Controller}},
      screen{new Display{DISPLAY::Width, DISPLAY::Height, ppu->getFrameBuffData(), p1Controller}}, dma_data{0x00},
      dma_dummy{true}
{
    soundQueue = new Sound_Queue;
    soundQueue->init(96000);
}

void NesSystem::newSamples(const blip_sample_t *samples, size_t count)
{
    soundQueue->write(samples, count);
}

void NesSystem::tick()
{
    ppu->run();

    if (systemClock % 3 == 0)
    {
        if (cpu->dma_transfer)
        {
            if (dma_dummy)
            {
                if (systemClock % 2 == 1)
                {
                    dma_dummy = false;
                }
            }
            else
            {
                if (systemClock % 2 == 0)
                {
                    dma_data = cpu->read(cpu->dma_page << 8 | cpu->dma_addr);
                }
                else
                {
                    ppu->pOAM[cpu->dma_addr++] = dma_data;
                    if (cpu->dma_addr == 0x00)
                    {
                        cpu->dma_transfer = false;
                        dma_dummy = true;
                    }
                }
            }
        }
        else
        {
            if (systemClock > 0)
            {
                --cpu->remaining;
            }
            cpu->fetch();
        }
    }

    if (ppu->requestCpuNmi)
    {
        cpu->nmi();
        ppu->requestCpuNmi = false;
    }
    ++systemClock;
}

void NesSystem::reset()
{
    cpu->reset();
    ppu->reset();
    systemClock = 0;
}

// #define DISASSEMBLE
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

void NesSystem::processGameplayInput(const SDL_Event &event)
{
    p1Controller->registerInputStateChange(event);
}