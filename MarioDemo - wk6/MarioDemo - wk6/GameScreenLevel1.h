#ifndef GAMESCREENLEVEL1_H
#define GAMESCREENLEVEL1_H

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"
#include "GameScreen.h"

class Texture2D;

class GameScreenLevel1 :
	public GameScreen {
private:
	Texture2D* mBackgroundTexture;
	SCREENS mScreen;

	bool SetUpLevel();

public:
	GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen);
	~GameScreenLevel1();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	SCREENS GetScreenID() { return mScreen; }
};

#endif // !GAMESCREENLEVEL1_H