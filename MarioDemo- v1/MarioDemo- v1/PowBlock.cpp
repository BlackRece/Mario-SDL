#include "PowBlock.h"

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map) :
	mRenderer(renderer), mLevelMap(map), mNumberOfHitsLeft(3),
	mSingleSpriteWidth(0), mSingleSpriteHeight(0), mTimer(0.0f) {

	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(BLOCK_PATH)) {
		std::cout << "Failed to load texture: " << BLOCK_PATH << std::endl;
		return;
	}

	mSingleSpriteWidth = int(mTexture->GetWidth() / 3);
	mSingleSpriteHeight = int(mTexture->GetHeight());

	mPositions.clear();
	if (!mLevelMap->GetAllTiles('0', mPositions)) {
		std::cout << "No POW positions in map!" << std::endl;
	}
}

PowBlock::~PowBlock() {
	mRenderer = nullptr;

	delete mTexture;
	mTexture = nullptr;

	mLevelMap = nullptr;
}

SDL_Rect PowBlock::GetCollisionBox(int index) {
	/*
	return mLevelMap->GetTileRect(
		mPositions[index].y+1, mPositions[index].x-1);
	*/

	return {
		int(mPositions[index].x * mSingleSpriteWidth),
		int(mPositions[index].y * mSingleSpriteHeight),
		(int)mSingleSpriteWidth,
		(int)mSingleSpriteHeight
	};
};

void PowBlock::Render() {
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (mNumberOfHitsLeft) {
	
		//Get the portion of the spritesheet that you want to draw.
		int left = int(mSingleSpriteWidth * (mNumberOfHitsLeft - 1));
	
		SDL_Rect portionOfSpritesheet = 
		{ left, 0, (int)mSingleSpriteWidth, (int)mSingleSpriteHeight };

		for (std::vector<Point2D>::iterator it = mPositions.begin();
			it != mPositions.end(); it++) {

			//Determine where you want it drawn.
			SDL_Rect destRect = { 
				int(it->x * mSingleSpriteWidth),
				int(it->y * mSingleSpriteHeight),
				(int)mSingleSpriteWidth,
				(int)mSingleSpriteHeight 
			};

			//Then draw it.
			mTexture->RenderRect(portionOfSpritesheet, destRect, flip);
		}
	}
}

void PowBlock::ResetHits() {
	if (mNumberOfHitsLeft <= 0) {
		for (std::vector<Point2D>::iterator it = mPositions.begin();
			it < mPositions.end(); it++) {

			mLevelMap->ChangeTileType(it->y, it->x, '0');
		}

		mNumberOfHitsLeft = 3;
	}
}

void PowBlock::TakeAHit() {
	if (mNumberOfHitsLeft <= 1) {
		//ensure zero hits left
		mNumberOfHitsLeft = 0;

		//remove/hide tile(s)
		for (std::vector<Point2D>::iterator it = mPositions.begin();
			it < mPositions.end(); it++) {

			mLevelMap->ChangeTileType(it->y, it->x);
		}

		//start reset timer
		mTimer = BLOCK_TIME;

	}
	else {
		mNumberOfHitsLeft--;
	}
}

void PowBlock::Update(float deltaTime) {
	if (mNumberOfHitsLeft < 1) {
		if (mTimer <= 0.0f) {
			ResetHits();
		} else {
			mTimer -= deltaTime;
		}
	}
}