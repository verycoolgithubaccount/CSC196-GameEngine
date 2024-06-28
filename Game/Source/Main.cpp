#include "Test.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	// create window
	// returns pointer to window if successful or nullptr if failed
	SDL_Window* window = SDL_CreateWindow("Game Engine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	while (true)
	{
		// clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// draw cube
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderDrawLine(renderer, 200, 100, 600, 100);
		SDL_RenderDrawLine(renderer, 200, 500, 600, 500);
		SDL_RenderDrawLine(renderer, 200, 100, 200, 500);
		SDL_RenderDrawLine(renderer, 600, 100, 600, 500);
		
		SDL_RenderDrawLine(renderer, 270, 170, 530, 170);
		SDL_RenderDrawLine(renderer, 270, 430, 530, 430);
		SDL_RenderDrawLine(renderer, 270, 170, 270, 430);
		SDL_RenderDrawLine(renderer, 530, 170, 530, 430);
		
		SDL_RenderDrawLine(renderer, 200, 100, 270, 170);
		SDL_RenderDrawLine(renderer, 600, 100, 530, 170);
		SDL_RenderDrawLine(renderer, 200, 500, 270, 430);
		SDL_RenderDrawLine(renderer, 600, 500, 530, 430);

		// clear the cube and draw random lines and dots instead
		//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		//SDL_RenderClear(renderer);
		for (int i = 0; i < 300; i++) {
			SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 0); // Random color
			SDL_RenderDrawLine(renderer, rand() % 800, rand() % 600, rand() % 800, rand() % 600); // Random line
			SDL_RenderDrawPoint(renderer, rand() % 800, rand() % 600); // Random point
		}

		// show screen
		SDL_RenderPresent(renderer);
	}

	return 0;
}