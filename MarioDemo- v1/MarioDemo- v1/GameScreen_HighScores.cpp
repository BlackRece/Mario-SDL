#include "GameScreen_HighScores.h"

GameScreen_HighScores::GameScreen_HighScores(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager) :
GameScreen(renderer, screen, manager), mScreen(screen), mBackImage(renderer) {
	char fontPath[] = "fonts/V5_bloques.ttf";
	if (!mScoreFont.LoadFont(renderer, fontPath, 28))
		std::cout << "Failed setting up font glyph array! (" <<
		fontPath << ")" << std::endl;


	/*
	show a background (scorlling level? or falling characters?)
	load scores
	display scores (scores should be sorted on save)
	wait for key or mouse press
		on press, return to title screen
	*/
}

GameScreen_HighScores::~GameScreen_HighScores() {

}

void GameScreen_HighScores::Render() {

	SDL_Color textCol = { 0xFF, 0xFF, 0xFF };

	SDL_Rect textRect = {
		TILE_WIDTH * 5, 
		16 + int(TILE_HEIGHT * 7),
		TILE_WIDTH * 6,
		TILE_HEIGHT * 5
	};

	/* highscore title */
	//14 wide x 16 tall
	mBGRect = {
		TILE_WIDTH * 4,
		TILE_HEIGHT * 1,
		TILE_WIDTH * 8,
		TILE_HEIGHT * 2
	};

	//render plate
	mBackImage.RenderPlate(mBGRect);

	//render text's shadow
	mBGRect.x--;
	mBGRect.y--;
	mScoreFont.RenderString(mBGRect, "High Scores", { 0xFF, 0x00, 0x00 });

	//render text
	mBGRect.x += 2;
	mBGRect.y += 2;
	mScoreFont.RenderString(mBGRect, "High Scores", textCol);

	/* highscores table */
	//scores background
	mBackImage.RenderPlate(textRect);

	//scores text
	textRect.h = 16;
	textRect.w += TILE_WIDTH;
	textRect.x -= int(TILE_WIDTH * 0.5f);

	std::string num;

	for (int i = 0; i < HighScores::Instance()->GetTotal(); i++) {
		textRect.y = 6 + (TILE_HEIGHT * 6) + (18 * i);
		num = std::to_string(i+1);
		if (num.length() == 1) {
			num = "0" + std::to_string(i+1);
		}
		mScoreFont.RenderString(
			textRect,
			num + ": " +
			HighScores::Instance()->GetScores(i),
			textCol
		);
	}

}

void GameScreen_HighScores::Update(float deltaTime, SDL_Event e) {
	//Handle any events.
	//Check for key releases.
	if (e.type == SDL_KEYUP) {
		
		//Check which key was released.
		switch (e.key.keysym.sym) {

			//Press 'ESCAPE' to quit.
			case SDLK_RETURN: case SDLK_RETURN2:
				SetNextScreen(SCREENS::SCREEN_INTRO);
			break;
		
		
		}
	
	}
}