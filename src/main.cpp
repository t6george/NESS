#include <memory>
#include <cstdint>
#include <string>
#include <iostream>
#include <stdio.h>
#include <RicohRP2C02.hpp>
#include <Apu2A03.hpp>

#include <SDL2/SDL.h>
#include <NesSystem.hpp>

int dmcRead(void *, unsigned int addr)
{
    return nes->cpu->read(addr);
}

enum emuState
{
    PLAY,
    RECORD_TAS,
    PLAY_TAS,
};

int main(int argc, char *argv[])
{
    assert(argc > 1);
    nes.reset(new NesSystem());
    Apu2A03 *apu = new Apu2A03;

    apu->func = dmcRead;
    apu->nes.reset(nes.get());
    nes->cpu->apu.reset(apu);
    nes->insertCartridge("smb.nes");

    // std::ofstream outputFile("test1.tas");
    // std::ifstream inputFile("test2.tas");
    bool quit = false;
    SDL_Event e;

    unsigned int frameStart, frameTime;
    const int FPS = 60;
    const int DELAY = 1000.0f / FPS;

    // enum emuState state = PLAY;
    // bool done = false;

    nes->cpu->bus->controller[0] = 0x00;
    while (!quit)
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
                quit = true;
                break;
            }
            else
            {
                nes->processGameplayInput(e);
            }

            // switch (e.type)
            // {
            // case SDL_QUIT:
            //     quit = true;
            //     break;
            // case SDL_KEYDOWN:
            //     switch (e.key.keysym.sym)
            //     {
            //     case SDLK_LEFT: // D-pad
            //         nes->cpu->bus->controller[0] |= 0x02;
            //         break;
            //     case SDLK_RIGHT:
            //         nes->cpu->bus->controller[0] |= 0x01;
            //         break;
            //     case SDLK_UP:
            //         nes->cpu->bus->controller[0] |= 0x08;
            //         break;
            //     case SDLK_DOWN:
            //         nes->cpu->bus->controller[0] |= 0x04;
            //         break;
            //     case SDLK_a: // A
            //         nes->cpu->bus->controller[0] |= 0x80;
            //         break;
            //     case SDLK_s: // B
            //         nes->cpu->bus->controller[0] |= 0x40;
            //         break;
            //     case SDLK_z: // Start
            //         nes->cpu->bus->controller[0] |= 0x10;
            //         break;
            //     case SDLK_x: // Select
            //         nes->cpu->bus->controller[0] |= 0x20;
            //         break;
            //     default:
            //         break;
            //     }
            //     break;
            // case SDL_KEYUP:
            //     switch (e.key.keysym.sym)
            //     {
            //     case SDLK_LEFT: // D-pad
            //         nes->cpu->bus->controller[0] &= ~0x02;
            //         break;
            //     case SDLK_RIGHT:
            //         nes->cpu->bus->controller[0] &= ~0x01;
            //         break;
            //     case SDLK_UP:
            //         nes->cpu->bus->controller[0] &= ~0x08;
            //         break;
            //     case SDLK_DOWN:
            //         nes->cpu->bus->controller[0] &= ~0x04;
            //         break;
            //     case SDLK_a: // A
            //         nes->cpu->bus->controller[0] &= ~0x80;
            //         break;
            //     case SDLK_s: // B
            //         nes->cpu->bus->controller[0] &= ~0x40;
            //         break;
            //     case SDLK_z: // Start
            //         nes->cpu->bus->controller[0] &= ~0x10;
            //         break;
            //     case SDLK_x: // Select
            //         nes->cpu->bus->controller[0] &= ~0x20;
            //         break;
            //     default:
            //         break;
            //     }
            //     break;
            // default:
            //     break;
            // }
            // break;
        }

        // }

        nes->cpu->remaining += FRAME_TICKS;
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