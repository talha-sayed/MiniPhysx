

#include <SDL.h>
#include <iostream>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


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
			windowSurface = SDL_GetWindowSurface(window);
		}
	}

	return success;
}


bool loadMedia()
{
	bool success = true;

	arrowSurfaces[SURFACE_DEFAULT] = SDL_LoadBMP("a.bmp");
	if(arrowSurfaces[SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_UP] = SDL_LoadBMP("images\\up.bmp");
	if(arrowSurfaces[SURFACE_UP] == NULL)
	{
		printf("Failed to load up image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_DOWN] = SDL_LoadBMP("images\\down.bmp");
	if(arrowSurfaces[SURFACE_DOWN] == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_LEFT] = SDL_LoadBMP("images\\left.bmp");
	if(arrowSurfaces[SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	arrowSurfaces[SURFACE_RIGHT] = SDL_LoadBMP("images\\right.bmp");
	if(arrowSurfaces[SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	return success;
}


SDL_Surface* loadImage(std::string filepath)
{

	SDL_Surface* image = SDL_LoadBMP(filepath.c_str());
	if(image == NULL)
	{
		printf("Error in loading the image file.\nSDL Error: %s", SDL_GetError());
	}

	return image;
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

				SDL_BlitSurface(currentSurface, NULL, windowSurface, NULL);

				SDL_UpdateWindowSurface(window);


				SDL_Delay(15);

			}

		}
	}


	close();

	return 0;
}
