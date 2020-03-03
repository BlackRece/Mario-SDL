#include "PowBlock.h"
#include <string>

using namespace std;

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map) {
	string imagePath = "images/PowBlock.png";
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str())) {
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}

	mLevelMap = map;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();
	mNumberOfHitsLeft = 3;
	mPosition = Vector2D((SCREEN_WIDTH * 0.5f) - mSingleSpriteWidth * 0.5f, 260);
}

PowBlock::~PowBlock() {
	mRenderer = nullptr;

	delete mTexture;
	mTexture = nullptr;

	mLevelMap = nullptr;
}

void PowBlock::Render() {
	if (mNumberOfHitsLeft) {
	
		//Get the portion of the spritesheet that you want to draw.
		int left = mSingleSpriteWidth * (mNumberOfHitsLeft - 1);
	
		//								{XPos, YPos, WidthfSingleSprite, HeightOfSingleSprite}
		SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

		//Determine where you want it drawn.
		SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

		//Then draw it.
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
}

void PowBlock::TakeAHit() {
	if (mNumberOfHitsLeft <= 0 ) {
		mNumberOfHitsLeft = 0;

		mLevelMap->ChangeTileAt(8, 7, 0);
		mLevelMap->ChangeTileAt(8, 8, 0);
	} else {
		mNumberOfHitsLeft--;
	}
}
