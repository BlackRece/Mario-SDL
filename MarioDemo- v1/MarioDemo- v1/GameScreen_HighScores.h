#ifndef GAMESCREEN_HIGHSCORES_H
#define GAMESCREEN_HIGHSCORES_H

/* STD lib */

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"
#include "GameScreen.h"
#include "TTF_Text.h"
#include "LevelMap.h"
#include "HighScores.h"

class GameScreenManager;

class GameScreen_HighScores :
	public GameScreen {
private:
	SCREENS			mScreen;
	TTF_Text		mScoreFont;
	LevelMap		mBackImage;
	SDL_Rect		mBGRect;

public:
	GameScreen_HighScores(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager);
	~GameScreen_HighScores();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;

};

#endif // GAMESCREEN_HIGHSCORES_H