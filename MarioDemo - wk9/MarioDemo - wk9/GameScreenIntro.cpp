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
	SetUpLevel();
}

GameScreenIntro::~GameScreenIntro() {
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

}

void GameScreenIntro::Render() {
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreenIntro::Update(float deltaTime, SDL_Event e) {
}
