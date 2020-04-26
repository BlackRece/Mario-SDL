#include "CharacterMario.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, 
	string imagePath, Vector2D startPosition, LevelMap* map, Colour alphaCol)
	: Character(renderer, imagePath, startPosition, map, alphaCol), mSize("tiny") {
	SetFacingDirection(FACING::FACING_RIGHT);

	//define size of sprites within spritesheet
	mSpriteSize.insert(pair<string, Point2D>("tiny", { 40,48 }));
	mSpriteSize.insert(pair<string, Point2D>("grow", { 40,56 }));
	mSpriteSize.insert(pair<string, Point2D>("tall", { 40,64 }));

	//define size of bounding box per sprite
	mBumpBox.insert(pair<string, SDL_Rect>("tiny", { 4, 4, 32, 44 }));
	mBumpBox.insert(pair<string, SDL_Rect>("grow", { 4, 4, 32, 52 }));
	mBumpBox.insert(pair<string, SDL_Rect>("tall", { 4, 4, 32, 60 }));

	//define sets of animation frames pair with sprite sizes
	Anime tmpAnim = {};
	tmpAnim.frames = 1;
	tmpAnim.pos = { 0,0 };
	mAnimInfo.insert(pair<string, Anime>("tiny-dead", tmpAnim));

	mAnimInfo.insert(pair<string, Anime>("tiny-idle", { 1, { 42, 00} }));
	mAnimInfo.insert(pair<string, Anime>("tiny-duck", { 1, { 84, 00} }));
	mAnimInfo.insert(pair<string, Anime>("tiny-jump", { 1, {126, 00} }));
	mAnimInfo.insert(pair<string, Anime>("tiny-run", { 3, { 00, 50} }));

	mAnimInfo.insert(pair<string, Anime>("grow-idle", { 1, { 00,150} }));

	mAnimInfo.insert(pair<string, Anime>("tall-idle", { 1, { 42,150} }));
	mAnimInfo.insert(pair<string, Anime>("tall-duck", { 1, { 84,150} }));
	mAnimInfo.insert(pair<string, Anime>("tall-jump", { 1, {126,150} }));
	mAnimInfo.insert(pair<string, Anime>("tall-run", { 3, { 00,216} }));

	mAnimInfo.insert(pair<string, Anime>("tall-idle-fire", { 1, { 00,282} }));
	mAnimInfo.insert(pair<string, Anime>("tall-run-fire", { 3, { 42,282} }));
	mAnimInfo.insert(pair<string, Anime>("tall-jump-fire", { 1, {168,282} }));

	mAction = "idle";
}

CharacterMario::~CharacterMario()
{
}

void CharacterMario::Render() {
	switch (GetFacingDirection()) {
		case FACING::FACING_LEFT:
			RenderSprite(SDL_FLIP_HORIZONTAL);
		break;
		case FACING::FACING_RIGHT:
			RenderSprite(SDL_FLIP_NONE);
		break;
		default: /* no other option */ break;
	}
}

void CharacterMario::Respawn(Vector2D startPos) {
	SetFacingDirection(FACING::FACING_RIGHT);
	SetPosition(startPos);
	Jump();
}

void CharacterMario::Update(float deltaTime, SDL_Event e) {
	UpdateSpriteInfo(mSize + "-" + mAction);

	Character::Update(deltaTime, e);

	//Handle any events.
	switch (e.type) {
		case SDL_KEYDOWN:
			//Check which key was pressed.
			switch (e.key.keysym.sym) {
				case SDLK_a:
					mMovingLeft = true;
					mMovingRight = false;
				break;

				case SDLK_d:
					mMovingRight = true;
					mMovingLeft = false;
				break;

				case SDLK_w: Jump(); break;
				case SDLK_s: mIsShooting = true; break;
			}
		break;

		case SDL_KEYUP:
			//Check which key was released.
			switch (e.key.keysym.sym) {
				case SDLK_a:	mMovingLeft = false; break;

				case SDLK_d:	mMovingRight = false; break;

				case SDLK_s:	mIsShooting = false; break;
			}
		break;
	}

	if (!mJumping) {
		if (mMovingLeft || mMovingRight) {
			mAction = "run";
		} else {
			mAction = "idle";
		}
	} else {
		mAction = "jump";
	}

	if (mIsShooting && mSize == "tall") {
		mAction.append("-fire");
	}
}
