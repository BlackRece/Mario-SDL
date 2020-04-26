#include "GameScreenIntro.h"

#include <iostream>
#include "Texture2D.h"

bool GameScreenIntro::SetUpLevel() {
	bool success = true;

	//Load the background texture.
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("images/Test.bmp")) {
		std::cout << "Failed to load background texture!" << std::endl;
		success = false;
	}

	return success;
}

GameScreenIntro::GameScreenIntro(SDL_Renderer* renderer, SCREENS screen) :
	GameScreen(renderer, screen) {
	mScreen = screen;
	if(!mTexts.LoadFont(mRenderer)) {
		std::cout << "\nFailed setting up font glyph array! :(" << std::endl;
	}
	
	mString.LoadFont(mRenderer);
	mString.LoadRenderedString("Boots and Cats!", { 0x00, 0x00, 0xFF });
	SetUpLevel();
}

GameScreenIntro::~GameScreenIntro() {
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

}

void GameScreenIntro::Render() {
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);

	SDL_Rect titlePos = {
		0,
		SCREEN_HEIGHT / 2,
		SCREEN_WIDTH,
		SCREEN_HEIGHT / 2
	};
	mTexts.RenderString(titlePos, "Super Mario Bros.", { 0xFF,0,0 });

	//mTexts.Render('A', { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });

	titlePos = { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
	mString.Render(titlePos);
}

void GameScreenIntro::Update(float deltaTime, SDL_Event e) {
}
