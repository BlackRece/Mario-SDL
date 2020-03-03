#include "GameScreen.h"

GameScreen::GameScreen(SDL_Renderer* renderer, SCREENS screen) {
	mRenderer = renderer;

	mScreen = screen;
}

GameScreen::~GameScreen() {
	mRenderer = nullptr;
}

void GameScreen::Render()
{
}

void GameScreen::Update(float deltaTime, SDL_Event e)
{
}

SCREENS GameScreen::GetScreenID() {
	return mScreen;
}
