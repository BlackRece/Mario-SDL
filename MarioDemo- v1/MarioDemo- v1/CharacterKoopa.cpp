#include "CharacterKoopa.h"

CharacterKoopa::CharacterKoopa(
	SDL_Renderer* renderer, string imagePath, LevelMap* map,
	Vector2D startPosition, FACING startFacing, float movementSpeed,
	Colour alphaCol) : Character(renderer, imagePath, startPosition, map, alphaCol),
	mValue(0), mSize("tall"), mAction("run"), mIsFlying(false) {
	SetFacingDirection(startFacing);
	SetMovementSpeed(movementSpeed);
	SetPosition(startPosition);

	mIsAlive = true;

	mInjured = false;
	mInjuredTime = 0.0f;

	//define size of sprites within spritesheet
	mSpriteSize.insert(pair<string, Point2D>("tiny", { 36, 32 }));
	mSpriteSize.insert(pair<string, Point2D>("tall", { 52, 52 }));

	//define size of bounding box per sprite
	mBumpBox.insert(pair<string, SDL_Rect>("tiny", { 4, 6, 28, 26 }));
	mBumpBox.insert(pair<string, SDL_Rect>("tall", { 4, 8, 44, 44 }));
	
	//define sets of animation frames pair with sprite sizes
	mAnimInfo.insert(pair<string, Anime>("tiny-idle", { 1, { 00, 54 } }));
	mAnimInfo.insert(pair<string, Anime>("tiny-spin", { 5, { 00, 54 } }));

	mAnimInfo.insert(pair<string, Anime>("tall-run", { 2, { 00, 00} }));
	mAnimInfo.insert(pair<string, Anime>("tall-fly", { 2, { 00, 88} }));

	mAnimInfo.insert(pair<string, Anime>("tall-run-turn", { 1, {108, 00} }));
	mAnimInfo.insert(pair<string, Anime>("tall-fly-turn", { 1, {108, 88} }));
}

void CharacterKoopa::FlipRightWayUp() {
	SetFacingDirection(
		(GetFacingDirection() == FACING::FACING_LEFT) ? FACING::FACING_RIGHT : FACING::FACING_LEFT
	);
	mInjured = false;
	Jump();
}

void CharacterKoopa::Jump() {
	if (mCanJump) {
		if (mIsFlying) {
			mJumpForce = INITIAL_JUMP_FORCE / 2;
		} else { 
			mJumpForce = INITIAL_JUMP_FORCE_SMALL; 
		}

		mJumping = true;
		mCanJump = false;
	}
}

void CharacterKoopa::Bounce() {
	//if (mCanJump) {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterKoopa::Render() {
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

	RenderSprite( flip );
}

void CharacterKoopa::TakeDamage() {
	mInjured = true;

	mInjuredTime = INJURED_TIME;

	Bounce();
}

void CharacterKoopa::Update(float deltaTime, SDL_Event e) {
	UpdateSpriteInfo(mSize + "-" + mAction);
	
	//To gain the benefit of the code written in the base class Update() functioon we need to call it.
	Character::Update(deltaTime, e);

	if (!mInjured) {
		//We are not injured so move.
		mSize = "tall";

		if (mIsFlying) {
			mAction = "fly";
			if (IsOnFloor()) {
				Jump();
			}
		} else {
			mAction = "run";
		}

		if (GetFacingDirection() == FACING::FACING_LEFT) {
			mMovingLeft		= true;
			mMovingRight	= false;
		} else {
			mMovingLeft		= false;
			mMovingRight	= true;
		}

	} else {
		//We should not be moving when injured.
		mSize = "tiny";

		if (mJumping) {
			mAction = "spin";
		} else {
			mAction = "idle";
		}

		mMovingLeft		= false;
		mMovingRight	= false;

		//Count down the injured time.
		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0f)
			FlipRightWayUp();
	}
}