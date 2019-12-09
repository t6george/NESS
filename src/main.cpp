#include <memory>
#include <cstdint>

#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>
#include <Display.hpp>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Display d(256, 240);
    d.blit();

    return 0;
}