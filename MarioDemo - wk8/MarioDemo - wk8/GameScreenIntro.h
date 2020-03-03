#ifndef GAMESCREENINTRO_H
#define GAMESCREENINTRO_H
/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"
#include "GameScreen.h"

class Texture2D; 

class GameScreenIntro :
	public GameScreen {
private:
	Texture2D* mBackgroundTexture;
	SCREENS mScreen;

	bool SetUpLevel();

public:
	GameScreenIntro(SDL_Renderer* renderer, SCREENS screen);
	~GameScreenIntro();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	SCREENS GetScreenID() { return mScreen; }

};


#endif // !GAMESCREENINTRO_H