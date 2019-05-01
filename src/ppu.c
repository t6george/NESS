#include <stdio.h>
#include <SDL2/SDL.h>

ppuNES* powerUpPpu (mainMemory* memory) {
	ppuNES* ppu;
	ppu->registerBase = memory->ppuRegs;
	ppu->registerBase[PPUSTATUS] = 0xA0;
	return ppu;
}

u8 ppuFlagGet (ppuNES* ppu, ppuReg reg, u8 lsb, u8 fieldWidth) {
	u8 mask = (1 << fieldWidth) - 1;
	return (ppu->registerBase[reg] & (mask << lsb)) >> lsb;
}

void ppuFlagSet (ppuNES* ppu, ppuReg reg, u8 lsb, u8 fieldWidth, u8 val) {
	u8 mask = (1 << fieldWidth) - 1;
	ppu->registerBase[reg] &= ~(mask << lsb);
	ppu->registerBase[reg] |= ((val & mask) << lsb);
}

int main (int argc, char* argv[]) {
	SDL_Window* window = NULL;
	window = SDL_CreateWindow
	(
	"NES", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED,
	640,
	480,
	SDL_WINDOW_SHOWN
	);

	SDL_Renderer* renderer = NULL;
	renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

	SDL_RenderClear( renderer );

	SDL_Rect r;
	r.x = 50;
	r.y = 50;
	r.w = 50;
	r.h = 50;

	SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

	SDL_RenderFillRect( renderer, &r );

	SDL_RenderPresent(renderer);

	SDL_Delay( 5000 );

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
