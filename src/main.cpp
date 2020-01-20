#include <memory>
#include <cstdint>
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <RicohRP2C02.hpp>
#include <Apu2A03.hpp>

#include <SDL2/SDL.h>
#include <NesSystem.hpp>
#include <dirent.h>
#include <unistd.h>

int dmcRead(void *, unsigned int addr)
{
    return nes->cpu->read(addr);
}

int main(int argc, char *argv[])
{
    nes.reset(new NesSystem());
    Apu2A03 *apu = new Apu2A03;

    apu->func = dmcRead;
    apu->nes.reset(nes.get());
    nes->cpu->apu.reset(apu);
    // chdir("..");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            printf("%s\n", ent->d_name);
        }
        closedir(dir);
    }
    else
    {
        perror("moss");
        return 0;
    }

    nes->insertCartridge("smb.nes");

    bool quit = false;
    SDL_Event e;

    unsigned int frameStart, frameTime;
    const int FPS = 60;
    const int DELAY = 1000.0f / FPS;

    // auto start = high_resolution_clock::now();
    nes->cpu->bus->controller[0] = 0x00;
    while (!quit)
    {
        frameStart = SDL_GetTicks();
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
                    nes->cpu->bus->controller[0] |= 0x10;
                    break;
                case SDLK_x: // Select
                    nes->cpu->bus->controller[0] |= 0x20;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT: // D-pad
                    nes->cpu->bus->controller[0] &= ~0x02;
                    break;
                case SDLK_RIGHT:
                    nes->cpu->bus->controller[0] &= ~0x01;
                    break;
                case SDLK_UP:
                    nes->cpu->bus->controller[0] &= ~0x08;
                    break;
                case SDLK_DOWN:
                    nes->cpu->bus->controller[0] &= ~0x04;
                    break;
                case SDLK_a: // A
                    nes->cpu->bus->controller[0] &= ~0x80;
                    break;
                case SDLK_s: // B
                    nes->cpu->bus->controller[0] &= ~0x40;
                    break;
                case SDLK_z: // Start
                    nes->cpu->bus->controller[0] &= ~0x10;
                    break;
                case SDLK_x: // Select
                    nes->cpu->bus->controller[0] &= ~0x20;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            break;
        }

        nes->cpu->remaining += 29781;
        while (nes->cpu->remaining > 0)
            nes->tick();
        apu->run_frame(nes->cpu->elapsed());
        nes->screen->blit(nes->cpu->bus->controller[0]);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < DELAY)
            SDL_Delay((int)(DELAY - frameTime));
    }

    return 0;
}
