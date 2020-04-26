#ifndef GAMESCREEN_H
#define GAMESCREEN_H

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"

class GameScreenManager;

class GameScreen {
protected:
	SDL_Renderer*		mRenderer;

private:
	SCREENS				mNextScreen;
	GameScreenManager*	mManager;

public:
	SCREENS				mScreen;

	GameScreen(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager);
	~GameScreen();

	virtual void		Render();
	virtual void		Update(float deltaTime, SDL_Event e);

	SCREENS				GetScreenID();
	SCREENS				GetNextScreen() { return mNextScreen; }
	void				GotoScreen(SCREENS newScreen);
	void				SetNextScreen(SCREENS nextScreen) { mNextScreen = nextScreen; }
};

#endif // !GAMESCREEN_H