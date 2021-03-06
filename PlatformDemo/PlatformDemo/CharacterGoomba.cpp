#include "CharacterGoomba.h"

CharacterGoomba::CharacterGoomba(
	SDL_Renderer* renderer, string imagePath, LevelMap* map,
	Vector2D startPosition, FACING startFacing, float movementSpeed)
	: Character(renderer, imagePath, startPosition, map) {
	SetFacingDirection(startFacing);
	SetMovementSpeed(movementSpeed);
	SetPosition(startPosition);

	mIsAlive = true;

	mInjured = false;
	mInjuredTime = 0.0f;

	mSingleSpriteWidth = (float)GetSpriteWidth();	
	mSingleSpriteHeight = (float)GetSpriteHeight() / 2;		//2 sprites on this spritesheet in 1 column.
}

void CharacterGoomba::FlipRightWayUp() {
	SetFacingDirection(
		(GetFacingDirection() == FACING::FACING_LEFT) ? FACING::FACING_RIGHT : FACING::FACING_LEFT
	);
	mInjured = false;
	Jump();	//TODO: ensure that the right jump function is being called.
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
	int top = 0;

	//If injured move the left position to be the left position of the second image on the spritesheet.
	if (mInjured)
		top = (int)mSingleSpriteHeight;

	//Get the portion of the spritesheet you want to draw.
	//
	SDL_Rect portionOfSpriteSheet = {
		0, top,												//XPos, YPos
		(int)mSingleSpriteWidth, (int)mSingleSpriteHeight	//WidthOfSingleSprite, HeightOfSingleSprite
	};

	//Determine where you want it drawn.
	SDL_Rect destRect = {
		(int)(mPosition.x), (int)(mPosition.y),				//XPos, YPos
		32, 32		//source file is a different size (not 32 x 32 pixels!
		//(int)mSingleSpriteWidth, (int)mSingleSpriteHeight	//WidthOfSingleSprite, HeightOfSingleSprite
	};

	//Then draw it facing the correct direction.
	Character::Render(
		&portionOfSpriteSheet, &destRect,
		(GetFacingDirection() == FACING::FACING_LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
	);
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
	}
	else {
		//We should not be moving when injured.
		mMovingLeft = false;
		mMovingRight = false;

		//Count down the injured time.
		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0f)
			FlipRightWayUp();
	}
}