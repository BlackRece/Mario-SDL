#include "CharacterCoin.h"
#include "Constants.h"

CharacterCoin::CharacterCoin(
	SDL_Renderer* renderer, string imagePath, LevelMap* map, 
	Vector2D startPosition)
	: Character(renderer, imagePath, startPosition, map) {
	SetPosition(startPosition);

	mValue = COIN_VALUE;
	mFrameDelay = 0.0f;
	mCurrentFrame = 0;
	mFrameForward = true;
	mIsAlive = true;

	mSingleSpriteWidth = (float)GetSpriteWidth() / COIN_FRAME_W;	//8 sprites on this spritesheet in 1 row.
	mSingleSpriteHeight = (float)GetSpriteHeight() / COIN_FRAME_H;
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
		(int)(mCurrentFrame * mSingleSpriteWidth), 0,		//XPos, YPos
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
		if (mFrameForward) {
			mCurrentFrame++;
		} else {
			mCurrentFrame--;
		}

		//Loop frame around if it goes beyond the number of frames.
		if (mCurrentFrame >= COIN_FRAME_W) {
			mCurrentFrame = 0;
			//mCurrentFrame = COIN_FRAME_W - 1;
			//mFrameForward = false;
		} else if (mCurrentFrame < 0) {
			mCurrentFrame = 0;
			mFrameForward = true;
		}
	}
}