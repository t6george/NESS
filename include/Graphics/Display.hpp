#pragma once
#include <cstdint>
#include <array>
#include <vector>

#include <HwConstants.hpp>
#include <SDL2/SDL.h>

class Display
{
    bool frameDrawn;
    uint16_t scanline;
    uint16_t cycle;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    // std::array<SDL_Color, DISPLAY::ColorCount> colors;

public:
    Display(const uint16_t width, const uint16_t height);
    ~Display() noexcept;
    void blit();
};