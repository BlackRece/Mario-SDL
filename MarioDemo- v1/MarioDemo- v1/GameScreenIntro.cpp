#include "GameScreenIntro.h"

#include <iostream>
#include "Texture2D.h"
#include "Collisions.h"

GameScreenIntro::GameScreenIntro(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager) :
	GameScreen(renderer, screen, manager), mBackGroundMap(renderer), mMouse({ 0,0 }) {
	mScreen = screen;
	if(!mTexts.LoadFont(mRenderer)) {
		std::cout << "\nFailed setting up font glyph array! :(" << std::endl;
	}
	
	mString.LoadFont(mRenderer);
	mString.LoadRenderedString("Boots and Cats!", { 0x00, 0x00, 0xFF });
	SetUpLevel();

	/* setup buttons */
	//mButtons[3];
	
	// option 1
	mButtons[0].text = "1: POW Panic";
	mButtons[0].rect = {
		TILE_WIDTH * 6, TILE_HEIGHT * 5,
		TILE_WIDTH * 4, int(TILE_HEIGHT * 1.5)
	};
	mButtons[0].screen = SCREENS::SCREEN_LEVEL1;

	// option 2
	mButtons[1].text = "2: Mario Runner";
	mButtons[1].rect = mButtons[0].rect;
	mButtons[1].rect.y = TILE_HEIGHT * 8;
	mButtons[0].screen = SCREENS::SCREEN_LEVEL2;

	// option 3
	mButtons[2].text = "3: High Scores";
	mButtons[2].rect = mButtons[0].rect;
	mButtons[2].rect.y = TILE_HEIGHT * 11;
	mButtons[0].screen = SCREENS::SCREEN_HIGHSCORES;

}

GameScreenIntro::~GameScreenIntro() {
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

}

void GameScreenIntro::Render() {
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);

	/* title */
	SDL_Rect titlePos = {
		TILE_WIDTH * 4,
		TILE_HEIGHT * 1,
		TILE_WIDTH * 8,
		TILE_HEIGHT * 2
	};
	
	RenderPlaque(titlePos, "Super Mario Demo", { 0x10, 0xC5, 0xC5 });

	/* option buttons */
	SDL_Color hoveredText = { 0x6B, 0xC5, 0x10 };

	for (int i = 0; i < 3; i++) {
		if (mButtons[i].onHover) {
			RenderPlaque(mButtons[i].rect, mButtons[i].text, hoveredText);
		} else {
			RenderPlaque(mButtons[i].rect, mButtons[i].text);
		}
	}

}

void GameScreenIntro::RenderPlaque(SDL_Rect plate_rect, std::string plate_string, SDL_Color text_col) {
	//render plate
	mBackGroundMap.RenderPlate(plate_rect);

	//render text's shadow
	plate_rect.x--;
	plate_rect.y--;
	mTexts.RenderString(plate_rect, plate_string, { 0xFF, 0x00, 0x00 });

	//render text
	plate_rect.x += 2;
	plate_rect.y += 2;
	mTexts.RenderString(plate_rect, plate_string, text_col);

}

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

void GameScreenIntro::Update(float deltaTime, SDL_Event e) {
	Point2D	clickPos;

	/* mouse inputs */
	if (e.type == SDL_MOUSEMOTION) {
		//Get mouse position
		SDL_GetMouseState(&mMouse.x, &mMouse.y);
	}

	if (e.type == SDL_MOUSEBUTTONUP) {
		/*
		if (SDL_GetMouseState(&clickPos.x, &clickPos.y) &
			SDL_BUTTON(SDL_BUTTON_LEFT)) {
			for (int i = 0; i < 3; i++) {
				if (Collisions::Instance()->PointInArea(clickPos, mButtons[i].rect)) {
					SetNextScreen(mButtons[i].screen);
				}
			}
		}
		*/

		if (e.button.button == SDL_BUTTON_LEFT) {
			clickPos.x = e.button.x;
			clickPos.y = e.button.y;

			for (int i = 0; i < 3; i++) {
				if (Collisions::Instance()->PointInArea(clickPos, mButtons[i].rect)) {
					SetNextScreen(mButtons[i].screen);
					//GameScreen::GotoScreen(mButtons[i].screen);
					break;
				}
			}
		}
	}

	//button being hovered?
	for (int i = 0; i < 3; i++) {
		if (Collisions::Instance()->PointInArea(mMouse, mButtons[i].rect)) 
			mButtons[i].onHover = true;
		else
			mButtons[i].onHover = false;
	}

}
