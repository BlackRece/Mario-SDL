#include "GameScreenManager.h"

/* usr lib */
#include "GameScreen.h"
#include "GameScreenIntro.h"
#include "GameScreenLevel1.h"
#include "GameScreen_HighScores.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen) {
	mRenderer = renderer;

	mCurrentScreen = nullptr;

	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager() {
	mRenderer = nullptr;
	
	delete mCurrentScreen;
	mCurrentScreen = nullptr;
}

void GameScreenManager::Render() {
	mCurrentScreen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e) {
	mCurrentScreen->Update(deltaTime, e);
	SCREENS nextScreen = mCurrentScreen->GetNextScreen();
	if (nextScreen != SCREENS::SCREEN_NULL)
		ChangeScreen(nextScreen);
	
}

void GameScreenManager::ChangeScreen(SCREENS newScreen) {
	//Clear up old screen.
	if (mCurrentScreen != nullptr) {
		delete mCurrentScreen;
	}

	switch (newScreen) {
		case SCREENS::SCREEN_INTRO:
			mCurrentScreen = (GameScreen*)(new GameScreenIntro(mRenderer, newScreen, this));
			break;
		case SCREENS::SCREEN_MENU:
			break;
		case SCREENS::SCREEN_LEVEL1:
			mCurrentScreen = (GameScreen*)(new GameScreenLevel1(mRenderer, newScreen, this));
			break;
		case SCREENS::SCREEN_LEVEL2:
			break;
		case SCREENS::SCREEN_GAMEOVER:
			break;
		case SCREENS::SCREEN_HIGHSCORES:
			mCurrentScreen = (GameScreen*)(new GameScreen_HighScores(mRenderer, newScreen, this));
			break;
		default:
			break;
	}
}

SCREENS GameScreenManager::GetCurrentScreen() {
	return mCurrentScreen->GetScreenID(); 
}
