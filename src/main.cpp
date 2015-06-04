

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


bool init();
bool loadMedia();
void close();
void printDebugInfo();
SDL_Texture* loadTexture(std::string);


SDL_Window* window = NULL;
SDL_Texture* bgTexture = NULL;
SDL_Texture* characterTexture = NULL;
SDL_Texture* spriteTexture = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect renderRect;
bool canJump;
bool isJump = false;

long counter = 0;

double integralY = 50;
double integralX = 320;

double deltaY = 0;
double deltaX = 0;

double accelY = 0;
double accelX = 0;

double gravForce = 0.00003;


double impactSpeed = 0;

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
			std::cout<<"Initialized correctly !"<<std::endl;

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

	bgTexture = loadTexture("images\\background.png");
	if(bgTexture == NULL)
	{
		printf("Failed to load default image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}

	characterTexture = loadTexture("images\\character.png");
	if(characterTexture == NULL)
	{
		printf("Failed to load map image.\nSDL Error: %s", SDL_GetError());
		success = false;
	}
	renderRect.w = 64;
	renderRect.h = 128;

	spriteTexture = loadTexture("images\\sprites.png");
	if(spriteTexture == NULL)
	{
		printf("Failed to load map image.\nSDL Error: %s", SDL_GetError());
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

		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0xFF, 0xFF));

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
	SDL_DestroyTexture(bgTexture);
	bgTexture = NULL;

	SDL_DestroyTexture(characterTexture);
	characterTexture = NULL;

	SDL_DestroyTexture(spriteTexture);
	spriteTexture = NULL;

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




void update()
{


	if(impactSpeed < 0.0002 && impactSpeed != 0)
	{
		impactSpeed = 0;
		deltaY = 0;
		integralY = 250;
	}

	if(integralY >= 250 && deltaY == 0 && impactSpeed != 0)
	{
		deltaY = -impactSpeed*0.4;

		std::cout << "deltaY: " << deltaY << " " << "impactSpeed: " << impactSpeed  << renderRect.h;
		std::cout << std::endl;
	}

	if(isJump)
	{
		if(integralY == 250)
		{
			deltaY = -0.08;
		}

		isJump = false;
	}


	deltaY += accelY + gravForce;
	deltaX += accelX;


	integralY += deltaY;
	integralX += deltaX;





	if(integralX >= 750)
	{
		integralX = 750;
		deltaX = 0;
	}

	if(integralX <= 0)
	{
		integralX = 0;
		deltaX = 0;
	}


	if(integralY >= 250)
	{
		integralY = 250;
		impactSpeed = deltaY;
		deltaY = 0;
	}
	if(integralY <= 0)
	{
		integralY = 0;
		deltaY = 0;
	}


	renderRect.y = integralY;
	renderRect.x = integralX;




	printDebugInfo();

	counter++;
}


void render()
{
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);






	SDL_RenderCopyEx(gRenderer, characterTexture, NULL, &renderRect, 0, NULL, SDL_FLIP_NONE );

	SDL_Rect clipRect = { 0, 100, 100, 100 };
	SDL_Rect posRect = {500, 200, 100, 100};
	SDL_RenderCopy(gRenderer, spriteTexture, &clipRect, &posRect);

	SDL_RenderPresent(gRenderer);

}


void printDebugInfo()
{
	if(counter % 10000 == 0)
	{
//		std::cout << "deltaY: " << deltaY << "  ";
//		std::cout << "deltaX: " << deltaX << " " << renderRect.h;
//		std::cout << std::endl;
		//std::cout<<"widthOffset: "<<widthOffset<<std::endl;
	}
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


					const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

					if(currentKeyStates[ SDL_SCANCODE_LEFT ] && currentKeyStates[ SDL_SCANCODE_RIGHT ])
					{
						deltaX = 0;
					}
					else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
					{
						deltaX = 0.05;
					}
					else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
					{
						deltaX = -0.05;
					}
					else
					{
						deltaX = 0;
						deltaY = 0;
					}

					if (currentKeyStates[ SDL_SCANCODE_SPACE])
					{
						 isJump = true;
					}



//					if(e.type == SDL_KEYDOWN)
//					{
//						switch(e.key.keysym.sym)
//						{
//						case SDLK_UP:
//						break;
//
//						case SDLK_DOWN:
//						break;
//
//						case SDLK_RIGHT:
//							deltaX = 0.05;
//						break;
//
//						case SDLK_LEFT:
//							deltaX = -0.05;
//						break;
//
//						case SDLK_SPACE:
//							isJump = true;
//						break;
//						}
//					}
//
//					if(e.type == SDL_KEYUP)
//					{
//						switch(e.key.keysym.sym)
//						{
//						case SDLK_UP:
//						break;
//
//						case SDLK_DOWN:
//						break;
//
//						case SDLK_RIGHT:
//							deltaX = 0;
//						break;
//
//						case SDLK_LEFT:
//							deltaX = 0;
//						break;
//						}
//					}
				}

//				if(renderRect.y >= 250) canJump = true;
//				else canJump = false;


				update();

				render();



				//SDL_Delay(1);
			}

		}
	}

	close();

	return 0;
}
