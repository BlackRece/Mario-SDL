/* Standard Lib */
#include <iostream>

/* SDL Lib */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/* Custom Lib */
#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"
#include "GameScreenManager.h"

using namespace::std;

/* Globals */
SDL_Window*			gWindow = nullptr;
SDL_Renderer*		gRenderer = nullptr;

GameScreenManager*	gScreenManager = nullptr;

Uint32				gOldTime;

/* Function Prototypes */
bool				InitSDL();
void				CloseSDL();
bool				Update();
void				Render();

int main(int argc, char* args[]) {
	//Flag to check if we wish to quit.
	bool quit = false;

	//Check if SDL was set up correctly.
	if (InitSDL()) {
		gScreenManager = new GameScreenManager(gRenderer, SCREENS::SCREEN_INTRO);

		gOldTime = SDL_GetTicks();

		//Game Loop.
		while (!quit) {
			Render();
			quit = Update();
		}
	}

	//Close Window and free resources.
	CloseSDL();

	return 0;
}

bool InitSDL() {
	//Flag to remember if something ahs failed.
	bool success = true;

	//Setup SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL did not initialise. Error: " << SDL_GetError();

		success = true;
	}
	else {
		//All good, so attempt to create the window.
		gWindow = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		//Did the window get created?
		if (gWindow == nullptr) {
			//Nope.
			cout << "Window was not created. Error: " << SDL_GetError();
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == nullptr) {
				cout << "Renderer was not created. Error: " << SDL_GetError();
				success = false;
			} else {
				//Initialise PNG loading.
				int imageFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imageFlags) && imageFlags)) {
					cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
					success = false;
				} else {
					/*
					//Load the background texture.
					gTexture = new Texture2D(gRenderer);
					if (!gTexture->LoadFromFile("images/test.bmp")) {
						cout << "Could not load image file (images/test.bmp). Error: " << endl;
						success = false;
					}
					*/
				}
			}
		}
	}

	return success;
}

void CloseSDL() {
	//Destroy the game screen manager.
	delete gScreenManager;
	gScreenManager = nullptr;

	//Release the renderer.
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	//Release the window.
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	//Quit SDL sub-systems.
	IMG_Quit();
	SDL_Quit();
}

bool Update() {
	//Flag to check if quiting game.
	bool success = false;

	//Get the new time.
	Uint32 newTime = SDL_GetTicks();

	//Event Handler.
	SDL_Event e;

	//Get the events.
	SDL_PollEvent(&e);

	//Handle any events.
	switch (e.type) {
		//Click the 'X' to quit.
	case SDL_QUIT:
		success = true;
		break;

		//Check for key releases.
	case SDL_KEYUP:
		//Check which key was released.
		switch (e.key.keysym.sym) {

			//Press 'ESCAPE' to quit.
		case SDLK_ESCAPE:
			success = true;
			break;
		case SDLK_a:
		case SDLK_d:
		case SDLK_w:
		case SDLK_s:
			/*
			create a player class
			must handle:
			- self animation
			- store its position
			*/
			break;
		case SDLK_SPACE:
			if (gScreenManager->GetCurrentScreen() == SCREENS::SCREEN_LEVEL1) {
				gScreenManager->ChangeScreen(SCREENS::SCREEN_INTRO);
			} else {
				gScreenManager->ChangeScreen(SCREENS::SCREEN_LEVEL1);
			}
			
			break;
		}
		break;

		//Check for mouse button presses.
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button) {
			//Left mouse button was pressed
		case 1:
			/*
			if (_gameState != gsPlaying) {
				_gameState = gsPlaying;
			}
			*/
			break;

			//Middle mouse button was pressed
		case 2:
			success = true;
			break;

			//Right mouse button was pressed
		case 3:
			success = true;
			break;
		}
		break;
	}

	gScreenManager->Update((float)(newTime - gOldTime) / 1000.0f, e);

	//Set the current time to be the old time.
	gOldTime = newTime;

	return success;
}

void Render() {
	//Clear the screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	gScreenManager->Render();

	//Update the screen
	SDL_RenderPresent(gRenderer);
}


