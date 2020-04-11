#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include <SDL_ttf.h>
#include <iostream>
#include "GameScreenManager.h"

using namespace::std;

//Globals
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
GameScreenManager* gameScreenManager;
Uint32 gOLdTime;

//Function prototypes
bool InitSDL();
void CloseSDL();
bool Update();
void Render();


int main(int argc, char* args[])
{
	if (InitSDL())
	{
		//Set up the game screen manager - Start with Level1
		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_LEVEL1);
		gOLdTime = SDL_GetTicks();
		//Pause for a few seconds
		//SDL_Delay(5000);

		//Flag to check if we wish to quit
		bool quit = false;

		//Game Loop
		while (!quit)
		{
			Render();
			quit = Update();
		}
	}

	//Close Window and free resources
	CloseSDL();

	return 0;
}

bool InitSDL()
{
	//Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise. ERROR: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		//All good, so attempt to create a window
		gWindow = SDL_CreateWindow("Super Mario Bros",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		//Did the window get created?
		if (gWindow == NULL)
		{
			//Nope
			cout << "Window was not created. EROOR: " << SDL_GetError() << endl;
			return false;
		}
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer != NULL)
	{
		//Initialise PNG loading
		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
			cout << "SDL_Image could not initialise. ERROR: " << IMG_GetError() << endl;
			return false;
		}
	}
	else
	{
		cout << "Renderer could not initialise. ERROR: " << SDL_GetError() << endl;
		return false;
	}

	//Initilaise the mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "Mixer could not initialise. Error: " << Mix_GetError();
		return false;
	}

	if (TTF_Init() == -1)
	{
		cout << "TTF could not initialise. Error: " << SDL_GetError();
		return false;
	}

	return true;
}

void CloseSDL()
{

	//Release the renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Destroy the game screen manager
	delete gameScreenManager;
	gameScreenManager = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void Render()
{
	//Clear the screen
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);

	gameScreenManager->Render();

	//Update the screen
	SDL_RenderPresent(gRenderer);
}

bool Update()
{
	//Get the new time
	Uint32 newTime = SDL_GetTicks();

	//Event handler
	SDL_Event e;

	//Get the events
	SDL_PollEvent(&e);

	//Handle any events
	switch (e.type)
	{
		//Click the 'X' to quit
		case SDL_QUIT:
			return true;
		break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
				//Press 'END' to quit
				case SDLK_END:
					return true;
				break;
			}
	}
	gameScreenManager->Update((float)(newTime - gOLdTime) / 1000.0f, e);
	//Set the current time to be the old time
	gOLdTime = newTime;
	return false;
}