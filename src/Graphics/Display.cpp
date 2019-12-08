#include <Display.hpp>

Display::Display(const uint16_t width, const uint16_t height)
    : frameDrawn{false}, scanline{0}, cycle{0}
{
    (void)frameDrawn;
    (void)scanline;
    (void)cycle;

    window = SDL_CreateWindow(
        "NES", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width * DISPLAY::PixelDim,
        height * DISPLAY::PixelDim,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect r;
    r.x = 50;
    r.y = 50;
    r.w = 50;
    r.h = 50;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_RenderFillRect(renderer, &r);

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

Display::~Display() noexcept
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}