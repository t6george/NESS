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
      screen{new Display{DISPLAY::Width, DISPLAY::Height, ppu->getFrameBuffData(), p1Controller}},
      soundQueue{new Sound_Queue()}, dma_data{0x00}, dma_dummy{true}, fps{60}, delay{1000 / fps}, delayMultiplier{1.0}
{
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

void NesSystem::outputFrame() const
{
    cpu->processFrameAudio();
    screen->blit();
}

bool NesSystem::run()
{
    frameStart = SDL_GetTicks();
    // if (state == RECORD_TAS)
    // {
    //     nes->cpu->bus->controller[0] = 0x00;
    //     bool out = false;
    //     while (!quit && !out)
    //     {
    //         while (SDL_PollEvent(&e))
    //         {
    //             switch (e.type)
    //             {
    //             case SDL_QUIT:
    //                 quit = true;
    //                 break;
    //             case SDL_KEYDOWN:
    //                 switch (e.key.keysym.sym)
    //                 {
    //                 case SDLK_LEFT: // D-pad
    //                     if ((nes->cpu->bus->controller[0] & 0x02) == 0x00)
    //                         outputFile << "L";
    //                     nes->cpu->bus->controller[0] |= 0x02;
    //                     break;
    //                 case SDLK_RIGHT:
    //                     if ((nes->cpu->bus->controller[0] & 0x01) == 0x00)
    //                         outputFile << "R";
    //                     nes->cpu->bus->controller[0] |= 0x01;
    //                     break;
    //                 case SDLK_UP:
    //                     if ((nes->cpu->bus->controller[0] & 0x08) == 0x00)
    //                         outputFile << "L";
    //                     nes->cpu->bus->controller[0] |= 0x08;
    //                     break;
    //                 case SDLK_DOWN:
    //                     if ((nes->cpu->bus->controller[0] & 0x04) == 0x00)
    //                         outputFile << "D";
    //                     nes->cpu->bus->controller[0] |= 0x04;
    //                     break;
    //                 case SDLK_a: // A
    //                     if ((nes->cpu->bus->controller[0] & 0x80) == 0x00)
    //                         outputFile << "A";
    //                     nes->cpu->bus->controller[0] |= 0x80;
    //                     break;
    //                 case SDLK_s: // B
    //                     if ((nes->cpu->bus->controller[0] & 0x40) == 0x00)
    //                         outputFile << "B";
    //                     nes->cpu->bus->controller[0] |= 0x40;
    //                     break;
    //                 case SDLK_z: // Start
    //                     if ((nes->cpu->bus->controller[0] & 0x10) == 0x00)
    //                         outputFile << "Z";
    //                     nes->cpu->bus->controller[0] |= 0x10;
    //                     break;
    //                 case SDLK_x: // Select
    //                     if ((nes->cpu->bus->controller[0] & 0x20) == 0x00)
    //                         outputFile << "X";
    //                     nes->cpu->bus->controller[0] |= 0x20;
    //                     break;
    //                 case SDLK_RETURN:
    //                     out = true;
    //                     break;
    //                 default:
    //                     break;
    //                 }
    //             default:
    //                 break;
    //             }
    //         }
    //     }

    //     if (nes->cpu->bus->controller[0] != 0x00)
    //         outputFile << std::endl;
    // }
    // else if (state == PLAY_TAS)
    // {
    //     nes->cpu->bus->controller[0] = 0x00;
    //     std::string inp;
    //     if (!done)
    //     {
    //         std::getline(inputFile, inp);

    //         for (int i = 0; i < inp.size(); ++i)
    //         {
    //             switch (inp[i])
    //             {
    //             case 'L': // D-pad
    //                 nes->cpu->bus->controller[0] |= 0x02;
    //                 break;
    //             case 'R':
    //                 nes->cpu->bus->controller[0] |= 0x01;
    //                 break;
    //             case 'U':
    //                 nes->cpu->bus->controller[0] |= 0x08;
    //                 break;
    //             case 'D':
    //                 nes->cpu->bus->controller[0] |= 0x04;
    //                 break;
    //             case 'A': // A
    //                 nes->cpu->bus->controller[0] |= 0x80;
    //                 break;
    //             case 'B': // B
    //                 nes->cpu->bus->controller[0] |= 0x40;
    //                 break;
    //             case 'Z': // Start
    //                 nes->cpu->bus->controller[0] |= 0x10;
    //                 break;
    //             case 'X': // Select
    //                 nes->cpu->bus->controller[0] |= 0x20;
    //                 break;
    //             default:
    //                 done = true;
    //                 break;
    //             }
    //         }
    //     }
    // }
    // else
    // {

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        else
        {
            processGameplayInput(e);
        }
    }

    // }

    cpu->restartFrameTimer();
    while (!cpu->isFrameDone())
        tick();
    outputFrame();

    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < static_cast<uint32_t>(delay * delayMultiplier))
        SDL_Delay((int)(delay - frameTime));

    return true;
}
