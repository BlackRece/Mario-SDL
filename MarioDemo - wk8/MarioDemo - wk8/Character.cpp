#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition) {
	mRenderer = renderer;

	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imagePath)) {
		cout << "Failed to load " << imagePath << " texture!" << endl;
	} else {

	}

	SetPosition(startPosition);

	mSpeed = MOVEMENT_SPEED;

	mFacingDirection = FACING::FACING_RIGHT;

	mMovingLeft = false;
	mMovingRight = false;

	mCanJump = true;
	mJumping = false;
	mJumpForce = 0.0f;

	mCollisionRadius = int(mTexture->GetWidth() + mTexture->GetHeight()) / 2;
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
		mPosition.y += 200.0f * deltaTime;
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

void Character::Render() {
	switch (mFacingDirection) {
	case FACING::FACING_LEFT:
		mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
		break;
	case FACING::FACING_RIGHT:
		mTexture->Render(mPosition, SDL_FLIP_NONE);
		break;
	default:
		break;
	}
}

void Character::SetPosition(Vector2D newPosition) {
	mPosition = newPosition;
}

void Character::Update(float deltaTime, SDL_Event e) {
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
		AddGravity(deltaTime);
	}

	if (mMovingLeft) {
		MoveLeft(deltaTime);
	} else if (mMovingRight) {
		MoveRight(deltaTime);
	}

}
