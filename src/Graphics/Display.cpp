#include <Display.hpp>

Display::Display(const uint16_t width, const uint16_t height, const uint32_t *fb)
    : frameDrawn{false}, scanline{0}, cycle{0},
      window{SDL_CreateWindow(
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
      frameBuffer{fb}
{
    (void)frameDrawn;
    (void)scanline;
    (void)cycle;
}

Display::~Display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::blit()
{

    // uint32_t x[256 * 240];

    // memset((void *)x, 0x8F, 256 * 240 * sizeof(uint32_t));

    // for (int i = 0; i < 4096; ++i)
    // {
    //     x[i] = 0xFFFF0000;
    // }
    SDL_UpdateTexture(texture, nullptr, frameBuffer, DISPLAY::Width * sizeof(uint32_t));

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, &canvas);
    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }
}