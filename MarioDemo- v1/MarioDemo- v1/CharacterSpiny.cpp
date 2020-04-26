#include "CharacterSpiny.h"


CharacterSpiny::CharacterSpiny(
	SDL_Renderer* renderer, string imagePath, LevelMap* map,
	Vector2D startPosition, FACING startFacing, float movementSpeed,
	Colour alphaCol) : Character(renderer, imagePath, startPosition, map, alphaCol),
	mValue(0), mSize("tiny"), mAction("run") {
	SetFacingDirection(startFacing);
	SetMovementSpeed(movementSpeed);
	SetPosition(startPosition);

	mIsAlive = true;

	mInjured = false;
	mInjuredTime = 0.0f;

	//define size of sprites within spritesheet
	mSpriteSize.insert(pair<string, Point2D>("tiny", { 40, 40 }));

	//define size of bounding box per sprite
	mBumpBox.insert(pair<string, SDL_Rect>("tiny", { 4, 6, 32, 34 }));
	
	//define sets of animation frames pair with sprite sizes
	mAnimInfo.insert(pair<string, Anime>("tiny-run",  { 2, {  0,  0 } }));
	mAnimInfo.insert(pair<string, Anime>("tiny-idle", { 2, { 84,  0 } }));
}

void CharacterSpiny::FlipRightWayUp() {
	SetFacingDirection(
		(GetFacingDirection() == FACING::FACING_LEFT) ? FACING::FACING_RIGHT : FACING::FACING_LEFT
	);
	mInjured = false;
	Jump();
}

void CharacterSpiny::Jump() {
	if (mCanJump) {
		//if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterSpiny::Render() {
	SDL_RendererFlip flip;

	if (mInjured) {
		if (GetFacingDirection() == FACING::FACING_LEFT) {
			flip = SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
		} else {
			flip = SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_NONE);
		}
	} else {
		if (GetFacingDirection() == FACING::FACING_LEFT) {
			flip = SDL_RendererFlip(SDL_FLIP_HORIZONTAL);
		} else {
			flip = SDL_RendererFlip(SDL_FLIP_NONE);
		}
	}

	RenderSprite(flip);
}

void CharacterSpiny::TakeDamage() {
	mInjured = true;

	mInjuredTime = INJURED_TIME;

	Jump();
}

void CharacterSpiny::Update(float deltaTime, SDL_Event e) {
	UpdateSpriteInfo(mSize + "-" + mAction);

	//To gain the benefit of the code written in the base class Update() functioon we need to call it.
	Character::Update(deltaTime, e);

	if (!mInjured) {
		//We are not injured so move.
		mSize = "tiny";

		mAction = "run";

		if (GetFacingDirection() == FACING::FACING_LEFT) {
			mMovingLeft = true;
			mMovingRight = false;
		} else {
			mMovingLeft = false;
			mMovingRight = true;
		}
	} else {
		//We should not be moving when injured.
		mSize = "tiny";

		if (mJumping) {
			mAction = "idle";
		}

		mMovingLeft = false;
		mMovingRight = false;

		//Count down the injured time.
		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0f)
			FlipRightWayUp();
	}
}