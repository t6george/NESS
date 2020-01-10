#include <memory>
#include <cstdint>
#include <string>
#include <chrono>
#include <iostream>
#include <RicohRP2C02.hpp>

#include <SDL2/SDL.h>
#include <NesSystem.hpp>

int main(int argc, char *argv[])
{
    std::unique_ptr<NesSystem> nes(new NesSystem());

    nes->insertCartridge("DonkeyKong.nes");

    bool quit = false;
    SDL_Event e;
    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        while (!nes->ppu->frame_complete)
            nes->tick();
        nes->ppu->frame_complete = false;
        nes->screen->blit();
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "FPS: " << (nes->getFrameCount() / duration.count()) << std::endl;

    return 0;
}
