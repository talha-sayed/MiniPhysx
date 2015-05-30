

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
SDL_Texture* loadTexture(std::string);


SDL_Window* window = NULL;
SDL_Texture* gTexture = NULL;
SDL_Renderer* gRenderer = NULL;

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
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if(gRenderer == NULL)
			{
				printf("Error in initializing SDL_Image library. \nSDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("Error in initializing SDL_Image library. \nSDL Error: %s\n", SDL_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


bool loadMedia()
{
	bool success = true;

	gTexture = loadTexture("images\\texture.png");
	if(gTexture == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	return success;
}


SDL_Texture* loadTexture(std::string filepath)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* image = IMG_Load(filepath.c_str());

	if(image == NULL)
	{
		printf("Error in loading the image file.\nSDL Error: %s", SDL_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, image);

		if(newTexture == NULL)
		{
			printf("Error in creating the texture.\nSDL Error: %s", SDL_GetError());
		}

		SDL_FreeSurface(image);

	}

	return newTexture;
}






void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(gRenderer);
	window = NULL;
	gRenderer = NULL;

	SDL_Quit();
}



void drawShapes()
{
	SDL_Rect fillRect = { SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
	SDL_RenderFillRect(gRenderer, &fillRect);

	SDL_Rect outlineRect = { SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3};
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xFF, 0xFF);
	SDL_RenderDrawRect(gRenderer, &outlineRect);

	SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);
	SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);
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

				SDL_RenderClear(gRenderer);

				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				drawShapes();

				SDL_RenderPresent(gRenderer);

				SDL_Delay(1);
			}

		}
	}

	close();

	return 0;
}
