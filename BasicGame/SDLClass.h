#pragma once

#include "Structures.h"

class SDLClass
{
	bool render;

	SDL_Window* window;
	SDL_Renderer* renderer;
	point windowSize;
	int refreshRate;

	point TakeWindowSize();
	void Resize();

public:
	bool exitGame;
	SDLClass();
	~SDLClass();
};

