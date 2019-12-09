#include <Display.hpp>

Display::Display(const uint16_t width, const uint16_t height)
    : frameDrawn{false}, scanline{0}, cycle{0},
      window{SDL_CreateWindow(
          "NES", SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          width * DISPLAY::PixelDim,
          height * DISPLAY::PixelDim,
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN)},
      renderer{SDL_CreateRenderer(
          window, -1,
          SDL_RENDERER_ACCELERATED)},
      pixels{std::vector<std::vector<Pixel>>(
          height, std::vector<Pixel>(
                      width, {{.x = 0x0, .y = 0x0, .w = DISPLAY::PixelDim, .h = DISPLAY::PixelDim},
                              {.r = 0x0, .g = 0x40, .b = 0x0, .a = 0xFF}}))}
{
    (void)frameDrawn;
    (void)scanline;
    (void)cycle;

    for (size_t y = 0; y < pixels.size(); ++y)
    {
        for (size_t x = 0; x < pixels[0].size(); ++x)
        {
            pixels[y][x].color.r = x;
            pixels[y][x].color.g = y;
            pixels[y][x].rect.x = x * DISPLAY::PixelDim;
            pixels[y][x].rect.y = y * DISPLAY::PixelDim;
        }
    }
}

Display::~Display() noexcept
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::blit()
{
    for (size_t y = 0; y < pixels.size(); ++y)
    {
        for (size_t x = 0; x < pixels[0].size(); ++x)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, pixels[y][x].color.r, pixels[y][x].color.g,
                                   pixels[y][x].color.b, pixels[y][x].color.a);

            SDL_RenderFillRect(renderer, &pixels[y][x].rect);

            SDL_RenderPresent(renderer);
        }
    }

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