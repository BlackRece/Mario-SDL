#include "CharacterKoopa.h"

CharacterKoopa::CharacterKoopa(
	SDL_Renderer* renderer, string imagePath, LevelMap* map,
	Vector2D startPosition, FACING startFacing, float movementSpeed)
	: Character(renderer, imagePath, startPosition, map) {
	SetFacingDirection(startFacing);
	SetMovementSpeed(movementSpeed);
	SetPosition(startPosition);

	mIsAlive = true;

	mInjured = false;
	mInjuredTime = 0.0f;

	mSingleSpriteWidth = (float)GetSpriteWidth() / 2;	//2 sprites on this spritesheet in 1 row.
	mSingleSpriteHeight = (float)GetSpriteHeight();
}

void CharacterKoopa::FlipRightWayUp() {
	SetFacingDirection(
		(GetFacingDirection() == FACING::FACING_LEFT) ? FACING::FACING_RIGHT : FACING::FACING_LEFT
	);
	mInjured = false;
	Jump();	//TODO: ensure that the right jump function is being called.
}


void CharacterKoopa::Jump() {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterKoopa::Render() {
	//Variable to hold the left position of the sprite we want to draw.
	int left = 0.0f;

	//If injured move the left position to be the left position of the second image on the spritesheet.
	if (mInjured)
		left = mSingleSpriteWidth;

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

	//Then draw it facing the correct direction.
	Character::Render(
		portionOfSpriteSheet, destRect,
		(GetFacingDirection() == FACING::FACING_LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
	);
}

void CharacterKoopa::TakeDamage() {
	mInjured = true;

	mInjuredTime = INJURED_TIME;

	Jump();
}

void CharacterKoopa::Update(float deltaTime, SDL_Event e) {
	//To gain the benefit of the code written in the base class Update() functioon we need to call it.
	Character::Update(deltaTime, e);

	if (!mInjured) {
		//We are not injured so move.
		if (GetFacingDirection() == FACING::FACING_LEFT) {
			mMovingLeft		= true;
			mMovingRight	= false;
		} else {
			mMovingLeft		= false;
			mMovingRight	= true;
		}
	} else {
		//We should not be moving when injured.
		mMovingLeft		= false;
		mMovingRight	= false;

		//Count down the injured time.
		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0f)
			FlipRightWayUp();
	}
}