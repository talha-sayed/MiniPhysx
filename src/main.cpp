

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1200;

const int JUMP_HEIGHT = 600;
const int GROUND_HEIGHT = 615;

bool init();
bool loadMedia();
void close();
void printDebugInfo();
SDL_Texture* loadTexture(std::string);
SDL_Texture* loadFontTexture(std::string, SDL_Color);


SDL_Window* window = NULL;
SDL_Texture* bgTexture = NULL;
SDL_Texture* characterTexture = NULL;
SDL_Texture* spriteTexture = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;
SDL_Texture *gTextTexture = NULL;

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

double gravForce = 0.13;


double impactSpeed = 0;


int textHeight = 0;
int textWidth = 0;

Uint32 elapsedFrames = 0;
Uint32 elapsedTicks = 0;


SDL_Point *pointsArray = NULL;
int pointIndex = 0;
bool showTrails = true;
const int NUM_POINTS = 1000;

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
		window = SDL_CreateWindow("MiniPhysx Demo Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

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


				if(TTF_Init() == -1)
				{
					printf("Could not initialize SDL_ttf. Error: %s\n", TTF_GetError());
					success = false;
				}

				pointsArray = new SDL_Point[NUM_POINTS];
				for(int i = 0; i < NUM_POINTS; i++)
				{
					pointsArray[i].x = pointsArray[i].y = -1;
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

	gFont = TTF_OpenFont("fonts\\UbuntuMono-R.ttf", 24);
	if(gFont == NULL)
	{
		printf("Failed to load ttf font.\n");
		success = false;
	}


	SDL_Color textColor = { 0, 0, 0};

	gTextTexture = loadFontTexture("Hello World !", textColor);
	if(gTextTexture == NULL)
	{
		printf("Failed to load font texture.\n");
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


SDL_Texture* loadFontTexture(std::string text, SDL_Color textColor)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* image = TTF_RenderText_Blended_Wrapped(gFont, text.c_str(), textColor, 150);

	if(image == NULL)
	{
		printf("Error in rendering the text to the surface. SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, image);

		if(newTexture == NULL)
		{
			printf("Error in creating the font texture.\nSDL Error: %s\n", SDL_GetError());
		}
		else
		{
			textHeight = image->h;
			textWidth = image->w;
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

	SDL_DestroyTexture(gTextTexture);
	gTextTexture = NULL;

	TTF_CloseFont(gFont);


	TTF_Quit();
	IMG_Quit();
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


int jumpIndex = 0;
Uint32 lastMeasured = 0;
Uint32 currentUpdateCount = 0;


bool isAirborne = false;

void update()
{

//	std::cout<<"Update is run"<<std::endl;


	Uint32 currentTicks = SDL_GetTicks();



	if(currentTicks >= lastMeasured + 1000)
	{
		lastMeasured += 1000;

		std::stringstream ss;

		ss<<"FPS: "<< currentUpdateCount <<"\n hello";

		if(gTextTexture != NULL)
		{
			SDL_DestroyTexture(gTextTexture);
		}

		SDL_Color textColor = { 0, 0, 0};

		gTextTexture = loadFontTexture( ss.str(), textColor);

		ss.str("");

		currentUpdateCount = 0;
	}


	if(impactSpeed < 0.0002 && impactSpeed != 0)
	{
		impactSpeed = 0;
		deltaY = 0;
		integralY = GROUND_HEIGHT;
	}

	if(integralY >= GROUND_HEIGHT && deltaY == 0 && impactSpeed != 0)
	{
		deltaY = -impactSpeed*0.4;

//		std::cout << "deltaY: " << deltaY << " " << "impactSpeed: " << impactSpeed  << renderRect.h;
//		std::cout << std::endl;
	}

	if(isJump)
	{
		if(integralY >= JUMP_HEIGHT)
		{
			deltaY = -7;
//			std::cout<<"Jumped "<< jumpIndex++ <<" times" << std::endl;
		}

		isJump = false;
	}


	deltaY += accelY + gravForce;
	deltaX += accelX;


	integralY += deltaY;
	integralX += deltaX;


	if(integralX >= SCREEN_WIDTH-50)
	{
		integralX = SCREEN_WIDTH-50;
		deltaX = 0;
	}

	if(integralX <= 0)
	{
		integralX = 0;
		deltaX = 0;
	}


	if(integralY >= GROUND_HEIGHT)
	{
		integralY = GROUND_HEIGHT;
		impactSpeed = deltaY;
		deltaY = 0;
	}
	if(integralY <= 0)
	{
		integralY = 0;
		deltaY = 0;
	}


	if(integralY >= JUMP_HEIGHT) canJump = true;
	else canJump = false;


	renderRect.y = integralY;
	renderRect.x = integralX;

	if(showTrails)
	{
		pointsArray[++pointIndex].x = integralX;
		pointsArray[pointIndex].y = integralY;

		if(pointIndex >= NUM_POINTS-1 ) pointIndex = 0;

	}


	printDebugInfo();

	currentUpdateCount++;
	counter++;
}


void render()
{
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);



	SDL_RenderDrawPoints(gRenderer, pointsArray, 1000);


	SDL_Rect clipRect = { 0, 100, 100, 100 };
	SDL_Rect posRect = {500, 200, 100, 100};
	SDL_RenderCopy(gRenderer, spriteTexture, &clipRect, &posRect);


	SDL_Rect fontRenderRect = { 550, 550, textWidth, textHeight};
	SDL_RenderCopy(gRenderer, gTextTexture, NULL, &fontRenderRect);

	SDL_RenderCopyEx(gRenderer, characterTexture, NULL, &renderRect, 0, NULL, SDL_FLIP_NONE );



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
					if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = true;
					}
				}

				//beginning code

				if(integralY < GROUND_HEIGHT) isAirborne = true;
				else isAirborne = false;


				if(deltaX*deltaX < 0.0001) deltaX = 0;
				//*********************

				const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );


				if(currentKeyStates[ SDL_SCANCODE_LEFT ] && currentKeyStates[ SDL_SCANCODE_RIGHT ])
				{
					//deltaX = 0;
				}
				else if (!currentKeyStates[ SDL_SCANCODE_LEFT ] && !currentKeyStates[ SDL_SCANCODE_RIGHT ])
				{
					if(integralY >= GROUND_HEIGHT)
					{
						deltaX = deltaX*0.999985;
					}
				}
				else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
				{
					if(isAirborne)
					{
						if(deltaX < 0) deltaX = deltaX*0.999999855;
						else deltaX = 5;
					}
					else deltaX = 5;
				}
				else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
				{
					if(isAirborne)
					{
						if(deltaX > 0) deltaX = deltaX*0.999999855;
						else deltaX = -5;
					}
					else deltaX = -5;
				}
				else
				{
//						deltaX = 0;
//						deltaY = 0;
				}

				if (currentKeyStates[ SDL_SCANCODE_SPACE] && canJump == true)
				{
					 isJump = true;
				}

				if (currentKeyStates[ SDL_SCANCODE_C])
				{
					 for(int i=0; i < NUM_POINTS; i++)
					 {
						 pointsArray[i].x = pointsArray[i].y = -1;
					 }
				}

				if (currentKeyStates[ SDL_SCANCODE_P])
				{
					 showTrails = false;
				}
				if (currentKeyStates[ SDL_SCANCODE_S])
				{
					 showTrails = true;
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




				elapsedFrames++;

				Uint32 currentTicks = SDL_GetTicks();



				if(currentTicks >= elapsedTicks + 10)
				{
					elapsedTicks += 10;

					update();

					render();
				}



			}

		}
	}

	close();

	return 0;
}
