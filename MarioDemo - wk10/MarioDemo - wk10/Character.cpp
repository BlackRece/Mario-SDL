#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map) {
	mRenderer = renderer;

	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imagePath)) {
		cout << "Failed to load " << imagePath << " texture!" << endl;
	} else {

	}

	SetPosition(startPosition);

	mSpeed = MOVEMENT_SPEED;

	mMovingLeft = false;
	mMovingRight = false;

	mCanJump = true;
	mJumping = false;
	mJumpForce = 0.0f;

	mCollisionRadius = int(mTexture->GetWidth() + mTexture->GetHeight()) / 2;
	mCurrentLevelMap = map;
}

Character::~Character() {
	mRenderer = nullptr;
}

void Character::AddGravity(float deltaTime) {
	if (mPosition.y+this->mTexture->GetHeight() >= SCREEN_HEIGHT ) {
		mCanJump = true;
		mJumping = false;
		mJumpForce = 0.0f;
	} else if (!mJumping) {
		mPosition.y += GRAVITY * deltaTime;
	}

}

Vector2D Character::GetPosition() {
	return mPosition;
}

float Character::GetCollisionRadius() {
	return mCollisionRadius;
}

Circle2D Character::GetCollisionCircle() {
	return Circle2D(mPosition.x, mPosition.y, mCollisionRadius);
}

void Character::CancelJump() {
	mJumping = false;
	mJumpForce = 0.0f;
}

Rect2D Character::GetCollisionBox() {
	return Rect2D(
		mPosition.x, mPosition.y, 
		mTexture->GetWidth(), mTexture->GetHeight()
	);
}

void Character::Jump() {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

void Character::MoveLeft(float deltaTime) {
	mPosition.x -= mSpeed;
	mFacingDirection = FACING::FACING_LEFT;
}

void Character::MoveRight(float deltaTime) {
	mPosition.x += mSpeed;
	mFacingDirection = FACING::FACING_RIGHT;
}

void Character::Render(SDL_RendererFlip flip) {
	mTexture->Render(mPosition, flip);
	/*
	switch (mFacingDirection) {
	case FACING::FACING_LEFT:
		break;
	case FACING::FACING_RIGHT:
		mTexture->Render(mPosition, SDL_FLIP_NONE);
		break;
	default:
		break;
	}
	*/
}

void Character::SetPosition(Vector2D newPosition) {
	mPosition = newPosition;
}

FACING Character::GetFacingDirection() {
	return mFacingDirection;
}

void Character::SetFacingDirection(FACING newDirection) {
	mFacingDirection = newDirection;
}

void Character::Update(float deltaTime, SDL_Event e) {
	//Collision position variables.
	int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;

	//additional movement?
	if (mJumping) {
		//Adjust the position.
		mPosition.y -= mJumpForce * deltaTime;

		//Reduce the jump force
		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;

		//Has the jump force reduced to zero?
		if (mJumpForce <= 0.0f)
			mJumping = false;
	} else {
		//Deal with gravity.
		if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0) {
			AddGravity(deltaTime);
		} else {
			//Collided with ground so we can jump again.
			mCanJump = true;
		}
	}

	if (mMovingLeft) {
		MoveLeft(deltaTime);
	} else if (mMovingRight) {
		MoveRight(deltaTime);
	}

}
