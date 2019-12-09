#pragma once
#include <cstdint>
#include <array>
#include <vector>

#include <HwConstants.hpp>
#include <SDL2/SDL.h>

class Display
{
    typedef struct
    {
        SDL_Rect rect;
        SDL_Color color;
    } Pixel;

    bool frameDrawn;
    uint16_t scanline;
    uint16_t cycle;

    SDL_Window *window;
    SDL_Renderer *renderer;

    // std::array<SDL_Color, DISPLAY::ColorCount> colors;

    std::vector<std::vector<Pixel>> pixels;

public:
    Display(const uint16_t width, const uint16_t height);
    ~Display() noexcept;
    void blit();
};