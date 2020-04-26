#include "CharacterLuigi.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map)
	: Character(renderer, imagePath, startPosition, map) {
	SetFacingDirection(FACING::FACING_LEFT);

	mSpriteWidth = 20;
	mSpriteHeight = 32;

	mFrameDelay = 0.0f;
	mCurrentFrame = 0;
	mFrameCount_Walk = 3;
	mFrameCount_Idle = 1;
	mFrameCount_Fire = 4;
	mFrameCount_FireIdle = 1;
	mFrameCount_Jump = 1;

	mMoving = false;

	mSpriteRect = new SDL_Rect();
	SDL_Rect sheetPos = {
		21, 108,
		mSpriteWidth, mSpriteHeight
	};

	mScreenRect = new SDL_Rect();
	SDL_Rect startPos = {
		(int)startPosition.x, (int)startPosition.y,
		mSpriteWidth, mSpriteHeight
	};
	mScreenRect = &startPos;
}

CharacterLuigi::~CharacterLuigi()
{
}

void CharacterLuigi::Render() {
	switch (GetFacingDirection()) {
	case FACING::FACING_LEFT:
		Character::Render(mSpriteRect, mScreenRect, SDL_FLIP_HORIZONTAL);
		//Character::Render(SDL_FLIP_HORIZONTAL);
		break;
	case FACING::FACING_RIGHT:
		Character::Render(mSpriteRect, mScreenRect, SDL_FLIP_NONE);
		//Character::Render(SDL_FLIP_NONE);
		break;
	default:
		break;
	}
}

void CharacterLuigi::SetState(bool newState) {
	if (newState) {
		SetFacingDirection(FACING::FACING_LEFT);
		SetPosition(Vector2D(SCREEN_WIDTH - 64, SCREEN_HEIGHT));
		Jump();
	}
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e) {
	Character::Update(deltaTime, e);

	//Handle any events.
	switch (e.type) {
	case SDL_KEYDOWN:
		//Check which key was pressed.
		switch (e.key.keysym.sym) {
		case SDLK_LEFT:
			mMovingLeft = true;
			mMovingRight = false;
			break;

		case SDLK_RIGHT:
			mMovingRight = true;
			mMovingLeft = false;
			break;

		case SDLK_UP:
			Jump();
			break;
		}
		break;

	case SDL_KEYUP:
		//Check which key was released.
		switch (e.key.keysym.sym) {

		case SDLK_LEFT:
			mMovingLeft = false;
			break;

		case SDLK_RIGHT:
			mMovingRight = false;
			break;
		}
		break;
	}

	if (mMovingLeft || mMovingRight) {
		if (!mJumping) {
			mMoving = true;
		} else {
			mMoving = false;
		}
	}


}
