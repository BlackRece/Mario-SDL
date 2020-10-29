#include "CharacterCoin.h"
#include "Constants.h"

CharacterCoin::CharacterCoin(SDL_Renderer* renderer, 
	string imagePath, LevelMap* map, Vector2D startPosition)
	: Character(renderer, imagePath, startPosition, map), mSize("norm") {
	SetPosition(startPosition);

	mAction = "idle";

	mSpriteSize.insert(pair<string, Point2D>("norm", { 32, 32 }));

	mAnimInfo.insert(pair<string, Anime>("norm-idle", { COIN_FRAME_W, { 00, 00 } }));

	mValue = COIN_VALUE;
	
	mMaxFrameDelay = ANIMATION_DELAY;
	mMaxFrames = COIN_FRAME_W;
	mAnimLoop = true;

	mIsAlive = true;
}

void CharacterCoin::Jump() {
	if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void CharacterCoin::Render(float offsetX) {
	RenderSprite(SDL_FLIP_NONE, offsetX);
}

void CharacterCoin::Update(float deltaTime, SDL_Event e) {
	UpdateSpriteInfo(mSize + "-" + mAction);

	Character::UpdateAnim(deltaTime, e);

}