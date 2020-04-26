#ifndef GAMESCREENINTRO_H
#define GAMESCREENINTRO_H
/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"
#include "GameScreen.h"
#include "TTF_Text.h"
#include "StringRender.h"
#include "LevelMap.h"

class Texture2D; 

class GameScreenIntro :
	public GameScreen {
private:
	Texture2D*		mBackgroundTexture;
	SCREENS			mScreen;
	TTF_Text		mTexts;
	StringRender	mString;
	LevelMap		mBackGroundMap;
	Option			mButtons[3];
	Point2D			mMouse;

	bool SetUpLevel();

public:
	GameScreenIntro(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager);
	~GameScreenIntro();

	void Render();
	void RenderPlaque(SDL_Rect plate_rect, std::string plate_string,
		SDL_Color text_col = { 0xFF, 0xFF, 0xFF });
	void Update(float deltaTime, SDL_Event e);

	SCREENS GetScreenID() { return mScreen; }

};


#endif // !GAMESCREENINTRO_H