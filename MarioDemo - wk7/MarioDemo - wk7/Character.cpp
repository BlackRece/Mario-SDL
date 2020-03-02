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

	/*
	//Handle any events.
	switch (e.type) {
	case SDL_KEYDOWN:
		//Check which key was pressed.
		switch (e.key.keysym.sym) {
		case SDLK_w:
		case SDLK_UP:
			cout << "JUMP!" << endl;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			mMovingLeft = true;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			mMovingRight = true;
			break;
		}
		break;
	case SDL_KEYUP:
		//Check which key was released.
		switch (e.key.keysym.sym) {

		case SDLK_ESCAPE:
		case SDLK_w:
		case SDLK_UP:
			Jump();
			break;
		case SDLK_s:
			break;
		case SDLK_a:
		case SDLK_LEFT:
			mMovingLeft = false;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			mMovingRight = false;
			break;
		case SDLK_SPACE:

			break;
		}
		break;

		//Check for mouse button presses.
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button) {
		//Left mouse button was pressed
		case 1:
			break;

		//Middle mouse button was pressed
		case 2:
			break;

		//Right mouse button was pressed
		case 3:
			break;
		}
		break;
	}
	*/

	
}
