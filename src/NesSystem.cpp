#include <fstream>

#include <NesSystem.hpp>
#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>
#include <GamePak.hpp>
#include <Display.hpp>
#include <HwConstants.hpp>
#include <Apu2A03.hpp>
#include <GamePad.hpp>

NesSystem::NesSystem(EmuState state, std::string outputPath)
    : systemClock{0}, p1Controller{new GamePad{}}, ppu{new RicohRP2C02{}}, cpu{new Ricoh2A03{ppu, p1Controller}},
      screen{new Display{DISPLAY::Width, DISPLAY::Height, ppu->getFrameBuffData(), p1Controller}},
      soundQueue{new Sound_Queue()}, dma_data{0x00}, dma_dummy{true}, fps{60}, delay{1000 / fps},
      delayMultiplier{1.0}, state{state}, commandI{0}, scriptPath{outputPath}
{
    soundQueue->init(96000);
    if (state == PLAY_TAS)
    {
        parseTasScript();
    }
}

NesSystem::~NesSystem() noexcept
{
    if (state == RECORD_TAS)
    {
        std::ofstream out;

        out.open(scriptPath, std::ofstream::binary);
        for (size_t i = 0; i < commands.size(); ++i)
        {
            out << commands[i];
        }
        out.close();
    }
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
                if ((systemClock & 0x1) == 1)
                {
                    dma_dummy = false;
                }
            }
            else
            {
                if ((systemClock & 0x1) == 0)
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

void NesSystem::parseTasScript()
{
    std::ifstream in;
    uint8_t frameInput;

    in.open(scriptPath, std::ifstream::binary);
    while (!in.eof())
    {
        in >> frameInput;
        commands.emplace_back(frameInput);
    }
    in.close();
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

void NesSystem::setGameplayInput(const uint8_t btns)
{
    p1Controller->setPressRegister(btns);
}

void NesSystem::saveGameplayInput()
{
    commands.emplace_back(p1Controller->readPressReg());
}

void NesSystem::outputFrame() const
{
    cpu->processFrameAudio();
    screen->blit();
}

bool NesSystem::run()
{
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&e))
    {
        if (e.type != SDL_QUIT)
        {
            switch (state)
            {
            case PLAY:
                processGameplayInput(e);
                break;
            case RECORD_TAS:
                while (SDL_PollEvent(&e) && (e.type != SDL_KEYDOWN || e.key.keysym.sym != SDLK_KP_ENTER))
                {
                    if (e.type == SDL_QUIT)
                    {
                        return false;
                    }
                    processGameplayInput(e);
                }
                saveGameplayInput();
                break;
            case PLAY_TAS:
                if (commandI < commands.size())
                {
                    setGameplayInput(commands[commandI++]);
                }
                break;
            default:
                break;
            }
        }
        else
        {
            return false;
        }
    }

    cpu->restartFrameTimer();
    while (!cpu->isFrameDone())
        tick();
    outputFrame();

    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < static_cast<uint32_t>(delay * delayMultiplier))
        SDL_Delay((int)(delay - frameTime));

    return true;
}
