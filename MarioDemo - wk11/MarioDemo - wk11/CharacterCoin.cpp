#include "CharacterCoin.h"

CharacterCoin::CharacterCoin(
	SDL_Renderer* renderer, string imagePath, LevelMap* map, 
	Vector2D startPosition)
	: Character(renderer, imagePath, startPosition, map) {
	SetPosition(startPosition);

	mValue = 0;
	mFrameDelay = 0.0f;
	mCurrentFrame = 0;
	mIsAlive = true;

	mSingleSpriteWidth = (float)GetSpriteWidth() / 3;	//3 sprites on this spritesheet in 1 row.
	mSingleSpriteHeight = (float)GetSpriteHeight();
}

void CharacterCoin::Jump() {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterCoin::Render() {
	//Get the portion of the spritesheet you want to draw.
	//
	SDL_Rect portionOfSpriteSheet = {
		(int)(mCurrentFrame * mSingleSpriteWidth), 0,				//XPos, YPos
		(int)mSingleSpriteWidth, (int)mSingleSpriteHeight	//WidthOfSingleSprite, HeightOfSingleSprite
	};

	//Determine where you want it drawn.
	SDL_Rect destRect = {
		(int)(mPosition.x), (int)(mPosition.y),				//XPos, YPos
		(int)mSingleSpriteWidth, (int)mSingleSpriteHeight	//WidthOfSingleSprite, HeightOfSingleSprite
	};

	//Then draw it.
	Character::Render(portionOfSpriteSheet, destRect, SDL_FLIP_NONE);
}

void CharacterCoin::Update(float deltaTime, SDL_Event e) {
	Character::Update(deltaTime, e);

	//Change frame?
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f) {
		//Reset frame delay count.
		mFrameDelay = ANIMATION_DELAY;

		//Move frame on.
		mCurrentFrame++;

		//Loop frame around if it goes beyond the number of frames.
		if (mCurrentFrame > 2) mCurrentFrame = 0;
	}
}