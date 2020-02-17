#include "GameScreenLevel1.h"

#include <iostream>
#include "Texture2D.h"

bool GameScreenLevel1::SetUpLevel() {
	bool success = true;

	//Load the background texture.
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("images/Test.bmp")) {
		std::cout << "Failed to load background texture!" << std::endl;
		success = false;
	}

	return success;
}

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen) :
	GameScreen(renderer, screen) {
	mScreen = screen;
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1() {
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

}

void GameScreenLevel1::Render() {
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {
}
