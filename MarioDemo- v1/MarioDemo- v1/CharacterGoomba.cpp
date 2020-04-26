#include "CharacterGoomba.h"

CharacterGoomba::CharacterGoomba(
	SDL_Renderer* renderer, string imagePath, LevelMap* map,
	Vector2D startPosition, FACING startFacing, float movementSpeed)
	: Character(renderer, imagePath, startPosition, map), 
	mSingleSpriteHeight(0), mSingleSpriteWidth(0) {
	SetFacingDirection(startFacing);
	SetMovementSpeed(movementSpeed);
	SetPosition(startPosition);

	mValue = 20;

	mIsAlive = true;

	mInjured = false;
	mInjuredTime = 0.0f;

}

void CharacterGoomba::FlipRightWayUp() {
	SetFacingDirection(
		(GetFacingDirection() == FACING::FACING_LEFT) ? FACING::FACING_RIGHT : FACING::FACING_LEFT
	);
	mInjured = false;
	Jump();
}


void CharacterGoomba::Jump() {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterGoomba::Render() {
	//Variable to hold the left position of the sprite we want to draw.
	int left = 0;

	//If injured move the left position to be the left position of the second image on the spritesheet.
	if (mInjured) left = (int)mSingleSpriteHeight;

	//Get the portion of the spritesheet you want to draw.
	//
	SDL_Rect portionOfSpriteSheet = {
		left, 0,											//XPos, YPos
		(int)mSingleSpriteWidth, (int)mSingleSpriteHeight	//WidthOfSingleSprite, HeightOfSingleSprite
	};

	//Determine where you want it drawn.
	SDL_Rect destRect = {
		(int)(mPosition.x), (int)(mPosition.y),				//XPos, YPos
		(int)mSingleSpriteWidth, (int)mSingleSpriteHeight	//WidthOfSingleSprite, HeightOfSingleSprite
	};

	SDL_RendererFlip flip;
	if (mInjured) {
		if (GetFacingDirection() == FACING::FACING_LEFT) {
			flip = SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		} else {
			flip = SDL_RendererFlip(SDL_FLIP_VERTICAL);
		}
	} else {
		flip = SDL_RendererFlip((GetFacingDirection() == FACING::FACING_LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	//Then draw it facing the correct direction.
	Character::Render(portionOfSpriteSheet, destRect, flip);
}

void CharacterGoomba::TakeDamage() {
	mInjured = true;

	mInjuredTime = INJURED_TIME;

	Jump();
}

void CharacterGoomba::Update(float deltaTime, SDL_Event e) {
	//To gain the benefit of the code written in the base class Update() functioon we need to call it.
	Character::Update(deltaTime, e);

	if (!mInjured) {
		//We are not injured so move.
		if (GetFacingDirection() == FACING::FACING_LEFT) {
			mMovingLeft = true;
			mMovingRight = false;
		}
		else {
			mMovingLeft = false;
			mMovingRight = true;
		}
	} else {
		//We should not be moving when injured.
		mMovingLeft = false;
		mMovingRight = false;

		//Count down the injured time.
		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0f)
			FlipRightWayUp();
	}
}