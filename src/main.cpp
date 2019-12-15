#include <memory>
#include <cstdint>
#include <string>

// #include <Ricoh2A03.hpp>
// #include <RicohRP2C02.hpp>
// #include <Display.hpp>
#include <SDL2/SDL.h>
#include <NesSystem.hpp>

int main(int argc, char *argv[])
{
    bool quit = false;
    SDL_Event e;
    std::unique_ptr<NesSystem> nes;

    // nes->insertCartridge("nestest.nes");

    SDL_Init(SDL_INIT_VIDEO);
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            nes->tick();
        }
    }

    return 0;
}