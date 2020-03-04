#ifndef GAMESCREENMANAGER_H
#define GAMESCREENMANAGER_H

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"

class GameScreen;

class GameScreenManager {
private:
	SDL_Renderer*	mRenderer;
	GameScreen*		mCurrentScreen;

public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	void ChangeScreen(SCREENS newScreen);
	SCREENS GetCurrentScreen();
};

#endif // !GAMESCREENMANAGER_H