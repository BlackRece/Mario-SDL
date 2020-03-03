#include "GameScreenManager.h"

/* usr lib */
#include "GameScreen.h"
#include "GameScreenIntro.h"
#include "GameScreenLevel1.h"

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
}

void GameScreenManager::ChangeScreen(SCREENS newScreen) {
	//Clear up old screen.
	if (mCurrentScreen != nullptr) {
		delete mCurrentScreen;
	}

	GameScreenLevel1* tmpLevel1Screen = nullptr;
	GameScreenIntro* tmpIntroScreen = nullptr;

	switch (newScreen) {
	case SCREENS::SCREEN_INTRO:
		tmpIntroScreen = new GameScreenIntro(mRenderer, newScreen);
		mCurrentScreen = (GameScreen*)tmpIntroScreen;
		tmpIntroScreen = nullptr;
		break;
	case SCREENS::SCREEN_MENU:
		break;
	case SCREENS::SCREEN_LEVEL1:
		tmpLevel1Screen = new GameScreenLevel1(mRenderer, newScreen);
		mCurrentScreen = (GameScreen*)tmpLevel1Screen;
		tmpLevel1Screen = nullptr;
		break;
	case SCREENS::SCREEN_LEVEL2:
		break;
	case SCREENS::SCREEN_GAMEOVER:
		break;
	case SCREENS::SCREEN_HIGHSCORES:
		break;
	default:
		break;
	}
}

SCREENS GameScreenManager::GetCurrentScreen() {
	return mCurrentScreen->GetScreenID(); 
}
