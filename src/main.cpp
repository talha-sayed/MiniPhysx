

#include <SDL.h>
#include <iostream>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;


	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error in initializing SDL. SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("MiniPhysx Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL)
		{
			printf("Error in creating the window. SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			surface = SDL_GetWindowSurface(window);

			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 150, 150, 255));

			SDL_UpdateWindowSurface(window);

			SDL_Delay(1000);

			SDL_Quit();
		}
	}

	return 0;
}
