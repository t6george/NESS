#include <Display.hpp>

Display::Display(const uint16_t width, const uint16_t height)
    : frameDrawn{false}, scanline{0}, cycle{0},
      window{SDL_CreateWindow(
          "NES", SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          width,
          height,
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN)},
      renderer{SDL_CreateRenderer(
          window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)},
      texture{SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ARGB8888,
          SDL_TEXTUREACCESS_STREAMING,
          width,
          height)}
{
    (void)frameDrawn;
    (void)scanline;
    (void)cycle;
}

Display::~Display() noexcept
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::blit()
{

    uint32_t x[256 * 240];
    // SDL_Rect src = {.x = 0, .y = 0, .w = 256 * 2, .h = 240 * 2};
    memset((void *)x, 0x8F, 256 * 240);
    SDL_UpdateTexture(texture, nullptr, x, 256);

    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 0xff);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
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

            // SDL_UpdateTexture(texture, nullptr, x, 256);

            // SDL_SetRenderDrawColor(renderer, 100, 0, 0, 0xff);
            // SDL_RenderClear(renderer);

            // SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            // SDL_RenderPresent(renderer);
        }
    }
}