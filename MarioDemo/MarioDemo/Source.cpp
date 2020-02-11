/* Standard Lib */
#include <iostream>

/* SDL Lib */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/* Custom Lib */
#include "Constants.h"

using namespace::std;

/* Globals */
SDL_Window* gWindow = nullptr;

/* Function Prototypes */
bool InitSDL();
void CloseSDL();
bool Update();

int main(int argc, char* args[]){
	//Flag to check if we wish to quit.
	bool quit = false;

	//Check if SDL was set up correctly.
	if (InitSDL()) {
		//Game Loop.
		while (!quit) {
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
	} else {
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
		}
	}

	return success;
}

void CloseSDL() {
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
			/*
			if (gameScreenManager->GetScreen() == SCREENS::sINTRO) {
				gameScreenManager->ChangeScreen(SCREENS::sLEVEL1);
			}
			else {
				gameScreenManager->ChangeScreen(SCREENS::sINTRO);
			}
			*/
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

	return success;
}