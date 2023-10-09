// BasicGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"

using namespace std;

//used global variable to avoid headache with additional mutexes
SDL_Window* window;
SDL_Renderer* renderer;
mutex m; //used during drawing to prevent 2 thread trying to acces the same data at the same time

void doo(int x, int width, int y, int height, int type, int w, int h)
{
//coordinates of model, {0,0} is center of window
	double CxMin = -2.5;
	double CxMax = 1.5; //not mirroring CX min because narrow left part of model(antenna)
	double CyMin = -2.0;
	double CyMax = CyMin * -1.0;

	double PixelWidth = (CxMax - CxMin) / w;
	double PixelHeight = (CyMax - CyMin) / h;

	double ER2 = 2; //how much detail 2 is good, 0,5 is nonexistent, 1,25 is loooking poor

	int Iteration = 0;
	int IterationMax = 300;
	double Cy, Cx, Zx, Zx2, Zy2, Zy;

	for (double iY = y; iY < height; iY++)
	{
		Cy = CyMin + iY * PixelHeight;
		if (fabs(Cy) < PixelHeight / 2) Cy = 0.0; /* Main antenna */

		for (double iX = x; iX < width; iX++)
		{
			Cx = CxMin + iX * PixelWidth;
			/* initial value of orbit = critical point Z= 0 */
			Zx = 0.0;
			Zy = 0.0;
			Zx2 = 0.0;
			Zy2 = 0.0;
			for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
			{
				Zy = 2 * Zx * Zy + Cy;
				Zx = Zx2 - Zy2 + Cx;
				Zx2 = Zx * Zx;
				Zy2 = Zy * Zy;

			};

			if (Iteration == IterationMax)
			{
				m.lock();
				SDL_SetRenderDrawColor(renderer, 0, type, 20, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawPoint(renderer, iX, iY);
				m.unlock();
			//	SDL_RenderPresent(renderer);
			}
			//else ((Zx2 + Zy2) < ER2) would be pixel beyone model
		}
	}
}


#include "SDLClass.h"


int main(int argc, char* args[])
{

	//#pragma region start


	int size = 600;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL_Init failed: %s\n", SDL_GetError());

		return -2;
	}
	window = SDL_CreateWindow("Manderbrod", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size, size, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_Delay(500);


	thread t1(doo, 0, size,
		int(size / 4), int(size / 2), 150, size, size);
	thread t2(doo, 0, size,
		int(size / 2), 3 * int(size / 4), 50, size, size);

	t1.join();
	t2.join();

	SDL_RenderPresent(renderer);
	SDL_Delay(20000);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	}