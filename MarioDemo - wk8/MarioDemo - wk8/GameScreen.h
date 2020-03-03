#ifndef GAMESCREEN_H
#define GAMESCREEN_H

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"

class GameScreen
{
protected:
	SDL_Renderer* mRenderer;

public:
	SCREENS mScreen;

	GameScreen(SDL_Renderer* renderer, SCREENS screen);
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	SCREENS GetScreenID();
};

#endif // !GAMESCREEN_H