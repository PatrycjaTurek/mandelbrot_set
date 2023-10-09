#include "SDLClass.h"

SDLClass::SDLClass()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		exitGame = true;
		return;
	};
	exitGame = false;
	windowSize = TakeWindowSize();
	window = SDL_CreateWindow("GameTM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_RESIZABLE);
	int i;
}

point SDLClass::TakeWindowSize()
{
	SDL_DisplayMode dm;// screen info in display mode
	SDL_GetCurrentDisplayMode(0, &dm);
	refreshRate = dm.refresh_rate;
	return { dm.w, dm.h };
}

SDLClass::~SDLClass()
{

}