#include <Display.hpp>
#include <iostream>
Display::Display(const uint16_t width, const uint16_t height, const uint32_t *fb)
    : window{SDL_CreateWindow(
          "NES", SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          width * DISPLAY::PixelDim,
          height * DISPLAY::PixelDim,
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN)},
      renderer{SDL_CreateRenderer(
          window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)},
      texture{SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ARGB8888,
          SDL_TEXTUREACCESS_STREAMING,
          width,
          height)},
      canvas{.x = 0, .y = 0, .w = width * DISPLAY::PixelDim, .h = height * DISPLAY::PixelDim},
      frameBuffer{fb} {}

Display::~Display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::blit()
{
    // std::cerr << "moss\n";
    SDL_UpdateTexture(texture, nullptr, frameBuffer, DISPLAY::Width * sizeof(uint32_t));

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, &canvas);
    SDL_RenderPresent(renderer);
}