#include <NesSystem.hpp>
#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>
#include <GamePak.hpp>

NesSystem::NesSystem()
    : cpu{new Ricoh2A03{}}, ppu{new RicohRP2C02{0x0008, 0x2000, 0x3FFF}} {}

void NesSystem::tick()
{
    ppu->run();

    if (clock % 3 == 0)
        cpu->fetch();
    ++clock;
}
void NesSystem::reset()
{
    cpu->reset();
    clock = 0;
}
void NesSystem::insertCartridge(const std::string &romName)
{
    std::shared_ptr<AddressableDevice> cart(new GamePak(romName, 0x4020, 0xFFFF));
    cpu->addCartridge(cart);
    ppu->addCartridge(cart);
}