#include "GameScreen.h"
#include "GameScreenManager.h"

GameScreen::GameScreen(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager) {
	mRenderer = renderer;
	mManager = manager;

	mScreen = screen;

	mNextScreen = SCREENS::SCREEN_NULL;
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

void GameScreen::GotoScreen(SCREENS newScreen) {
	mManager->ChangeScreen(newScreen);
}
