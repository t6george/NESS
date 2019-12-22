#include <NesSystem.hpp>
#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>
#include <GamePak.hpp>
#include <Display.hpp>
#include <HwConstants.hpp>

NesSystem::NesSystem()
    : systemClock{0}, ppu{new RicohRP2C02{}}, cpu{new Ricoh2A03{ppu}},
      screen{new Display{DISPLAY::Width, DISPLAY::Height, ppu->getFrameBuffData()}} {}

void NesSystem::tick()
{
    ppu->run();

    if (systemClock % 3 == 0)
    {
        cpu->fetch();
    }

    if (ppu->requestCpuNmi)
    {
        cpu->nmi();
        ppu->requestCpuNmi = false;
    }

    if (systemClock % (256 * 240) == 0)
    {
        screen->blit();
    }

    ++systemClock;

    
}

void NesSystem::reset()
{
    cpu->reset();
    ppu->reset();
    systemClock = 0;
}

void NesSystem::insertCartridge(const std::string &romName)
{
    std::shared_ptr<AddressableDevice> cart(new GamePak(romName));
    cpu->addCartridge(cart);
    ppu->addCartridge(cart);
}