

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


bool init();
bool loadMedia();
void close();
SDL_Surface* loadImage(std::string);


SDL_Window* window = NULL;
SDL_Surface* windowSurface = NULL;
SDL_Surface* imageSurface = NULL;



enum keypressSurfaces {
	SURFACE_DEFAULT,
	SURFACE_UP,
	SURFACE_DOWN,
	SURFACE_LEFT,
	SURFACE_RIGHT,
	SURFACE_TOTAL

};


SDL_Surface* arrowSurfaces[SURFACE_TOTAL];



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

			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("Error in initializing SDL_Image library. \nSDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				windowSurface = SDL_GetWindowSurface(window);
			}
		}
	}

	return success;
}


bool loadMedia()
{
	bool success = true;

	arrowSurfaces[SURFACE_DEFAULT] = IMG_Load("images\\loaded.png");
	if(arrowSurfaces[SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_UP] = loadImage("images\\up.bmp");
	if(arrowSurfaces[SURFACE_UP] == NULL)
	{
		printf("Failed to load up image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_DOWN] = loadImage("images\\down.bmp");
	if(arrowSurfaces[SURFACE_DOWN] == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_LEFT] = loadImage("images\\left.bmp");
	if(arrowSurfaces[SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_RIGHT] = loadImage("images\\right.bmp");
	if(arrowSurfaces[SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	return success;
}


SDL_Surface* loadImage(std::string filepath)
{
	SDL_Surface* optimizedImage = NULL;

	SDL_Surface* image = SDL_LoadBMP(filepath.c_str());
	if(image == NULL)
	{
		printf("Error in loading the image file.\nSDL Error: %s", SDL_GetError());
	}
	else
	{
		optimizedImage = SDL_ConvertSurface(image, windowSurface->format, NULL);

		if(optimizedImage == NULL)
		{
			printf("Error in creating the optimized image.\nSDL Error: %s", SDL_GetError());
		}

		SDL_FreeSurface(image);

	}

	return optimizedImage;
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


			SDL_Surface* currentSurface = arrowSurfaces[SURFACE_DEFAULT];



			while(!quit)
			{
				while(SDL_PollEvent(&e) != 0)
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if(e.type == SDL_KEYDOWN)
					{
						switch(e.key.keysym.sym)
						{
							case SDLK_UP:
							currentSurface = arrowSurfaces[SURFACE_UP];
							break;

							case SDLK_DOWN:
							currentSurface = arrowSurfaces[SURFACE_DOWN];
							break;

							case SDLK_LEFT:
							currentSurface = arrowSurfaces[SURFACE_LEFT];
							break;

							case SDLK_RIGHT:
							currentSurface = arrowSurfaces[SURFACE_RIGHT];
							break;

							default:
							currentSurface = arrowSurfaces[SURFACE_DEFAULT];
							break;
						}
					}
					else
					{
						currentSurface = arrowSurfaces[SURFACE_DEFAULT];
					}

				}

				SDL_Rect stretchRect;

				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.h = SCREEN_HEIGHT;
				stretchRect.w = SCREEN_WIDTH;

				SDL_BlitScaled(currentSurface, NULL, windowSurface, &stretchRect);

				SDL_UpdateWindowSurface(window);

				SDL_Delay(15);

			}

		}
	}


	close();

	return 0;
}
