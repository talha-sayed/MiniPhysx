

#include <SDL.h>
#include <iostream>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init();

bool loadMedia();

void close();



SDL_Window* window = NULL;
SDL_Surface* windowSurface = NULL;
SDL_Surface* imageSurface = NULL;



bool init()
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error in initializing SDL. SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		window = SDL_CreateWindow("MiniPhysx Demo Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL)
		{
			printf("Error in creating window surface. SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			windowSurface = SDL_GetWindowSurface(window);
		}
	}

	return success;
}


bool loadMedia()
{
	bool success = true;

	imageSurface = SDL_LoadBMP("a.bmp");

	if(imageSurface == NULL)
	{
		printf("Error in loading the image file.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	SDL_FreeSurface(imageSurface);
	imageSurface = NULL;

	SDL_DestroyWindow(window);
	SDL_Quit();
}



int main(int argc, char* argv[])
{

	if(!init())
	{
		printf("Failed to initialize.\n");
	}
	else
	{
		if(!loadMedia())
		{
			printf("Failed to load media.\nSDL Error: %s", SDL_GetError());
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			while(!quit)
			{
				while(SDL_PollEvent(&e) != 0)
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}

				}

				SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);

				SDL_UpdateWindowSurface(window);
			}

		}
	}


	close();

	return 0;
}
