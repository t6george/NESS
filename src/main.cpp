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

    nes->insertCartridge("nestest.nes");

    bool quit = false;
    SDL_Event e;
    using namespace std::chrono;

    // auto start = high_resolution_clock::now();
    while (!quit)
    {
        nes->cpu->bus->controller[0] = 0x00;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT: // D-pad
                    nes->cpu->bus->controller[0] |= 0x02;
                    break;
                case SDLK_RIGHT:
                    nes->cpu->bus->controller[0] |= 0x01;
                    break;
                case SDLK_UP:
                    nes->cpu->bus->controller[0] |= 0x08;
                    break;
                case SDLK_DOWN:
                    nes->cpu->bus->controller[0] |= 0x04;
                    break;
                case SDLK_a: // A
                    nes->cpu->bus->controller[0] |= 0x80;
                    break;
                case SDLK_s: // B
                    nes->cpu->bus->controller[0] |= 0x40;
                    break;
                case SDLK_z: // Start
                    nes->cpu->bus->controller[0] |= 0x20;
                    break;
                case SDLK_x: // Select
                    nes->cpu->bus->controller[0] |= 0x10;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        while (!nes->ppu->frame_complete)
            nes->tick();
        nes->ppu->frame_complete = false;
        nes->screen->blit();
    }
    // auto stop = high_resolution_clock::now();
    // auto duration = duration_cast<seconds>(stop - start);
    // std::cout << "FPS: " << (nes->getFrameCount() / duration.count()) << std::endl;

    return 0;
}
