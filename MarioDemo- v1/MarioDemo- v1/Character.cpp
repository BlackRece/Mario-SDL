#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"
#include "Collisions.h"

Character::Character(SDL_Renderer* renderer,
	string imagePath, Vector2D startPosition, LevelMap* map, Colour alphaCol) :
	mFrameDelay(0.0f), mMaxFrameDelay(ANIMATION_DELAY), mCurrentFrame(0), mMaxFrames(0),
	mFrameForward(true), mAnimLoop(true), mIsShooting(false), mAnimID("-")
{
	mRenderer = renderer;

	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imagePath, alphaCol.cRed, alphaCol.cGreen, alphaCol.cBlue)) {
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

	//TODO: fix - based on texture size
	/*
	needs to be told:
		size of single sprite
		tiny = 40 x 48
		grow = 40 x 56
		tall = 40 x 64

		number of rows in spritesheet (int) don't need?
		number of frames in each row of spritesheet? don't need?
		sequence of frames per action (e.g. walking, running, jumping, idle etc)
		tiny dead = 1 frame = 0,0
		tiny idle = 1 frame = 1,0
		tiny duck = 1 frame = 2,0
		tiny jump = 1 frame = 3,0
		tiny run  = 3 frame = 0-2,1

		grow idle = 1 frame = 0,3

		tall idle = 1 frame = 1,3
		tall duck = 1 frame = 2,3
		tall jump = 1 frame = 3,3
		tall run  = 3 frame = 0-2,4

		tall idle-fire = 1 frame = 0,5
		tall walk-fire = 3 frame = 1-3,5
		tall jump-fire = 1 frame = 4,5

		one struct:
		struct Anim {
			sprite_size (string) - id from map<string, Point2D>.first
			frames (int) - no of frames
			pos (Point2D) - position of first frame
		}
		
		two arrays:
		map<string, Anim>
			action_id (string) - name of animation
			Anim (struct) 

		map<string, Point2D> spriteSize
			size_id (string) - name of sprite size
			dimensions (Point2D) - width and height of sprite on sheet


		how to calc collision box and circle (from single sprite size)
	*/

	mAnimInfo.clear();
	mSpriteSize.clear();
	mBumpBox.clear();
	
	mCurrentLevelMap = map;
}

Character::~Character() {
	mRenderer = nullptr;
}

void Character::AddGravity(float deltaTime) {
	if (mPosition.y + mSpriteSize[mAnimID].y >= SCREEN_HEIGHT ) {
		mCanJump = true;
		mJumping = false;
		mJumpForce = 0.0f;
	} else if (!mJumping) {
		mPosition.y += GRAVITY * deltaTime;
	}

}

void Character::CancelJump() {
	mJumping = false;
	mJumpForce = 0.0f;
	mPosition.y += mSpeed;	//stop character being stuck in pow block
}

SDL_Rect Character::GetCollisionBox() {
	/*
	string tmpSize = GetSpriteSize(mAnimID);
	SDL_Rect result = {
		int(mPosition.x),// + mBumpBox[tmpSize].x),
		int(mPosition.y),// + mBumpBox[tmpSize].y),
		mBumpBox[tmpSize].w,
		mBumpBox[tmpSize].h
	};
	*/

	SDL_Rect result = GetSpriteRect();
	return result;
}

float Character::GetCollisionRadius() {
	return float(mBumpBox[GetSpriteSize(mAnimID)].w / 2);
}

//bounding circle from centre of sprite
Circle2D Character::GetCollisionCircle() {
	/*
	return Circle2D(
		float(mPosition.x + mBumpBox[mAnimID].x + mBumpBox[mAnimID].w / 2),
		float(mPosition.y + mBumpBox[mAnimID].y + mBumpBox[mAnimID].h / 2),
		float(mBumpBox[mAnimID].h / 2));
	*/

	return Circle2D(
		mPosition.x, mPosition.y,
		GetCollisionRadius());
}

Vector2D Character::GetPosition() {
	return mPosition;
}

//return animation struct object
Anime Character::GetSpriteAnime() {
	Anime result = { 0 };

	map<string, Anime>::iterator it;
	for (it = mAnimInfo.begin(); it != mAnimInfo.end(); it++) {
		if (it->first == mAnimID) {
			result = it->second;
			break;
		}
	}

	return result;
}

//return maximum number of frames for animation
int Character::GetSpriteFrames() {
	int result = 0;

	Anime tmpAnime = GetSpriteAnime();
	if (tmpAnime.frames != 0) {
		result = tmpAnime.frames;
	}

	return result;
}

SDL_Rect Character::GetSpriteRect() {
	SDL_Rect result{0};
	string tmpSize;

	Anime tmpAnime = GetSpriteAnime();
	tmpSize = GetSpriteSize(mAnimID);
	if (tmpAnime.frames != 0) {
		result = {
			(int)mPosition.x,
			(int)mPosition.y,
			mSpriteSize[tmpSize].x,
			mSpriteSize[tmpSize].y
		};
	} else {
		//DEBUG: invalid animation
		/*
		occurs on first loop before default vars are updated
		std::cout << "Error: invalid animation! " <<
			mAnimID << std::endl;
		*/
	}

	/*
	map<string, Anime>::iterator it;
	for (it = mAnimInfo.begin(); it != mAnimInfo.end(); it++) {
		if (it->first == anim_id) {
			tmpSize = GetSpriteSize(anim_id);
			result = {
				it->second.pos.x + ((mSpriteSize[tmpSize].x + 2) * frame_no),
				it->second.pos.y,
				mSpriteSize[tmpSize].x,
				mSpriteSize[tmpSize].y
			};
			break;
		}
	}
	*/
	
	return result;
}

string Character::GetSpriteSize(string size_id) {
	string result;

	result = size_id.substr(0, size_id.find_first_of('-'));

	return result;
}

bool Character::IsOnFloor() {
	
	//get foot position
	int footPosition = (int)(
		mPosition.y + 
		float(mBumpBox[GetSpriteSize(mAnimID)].y) +
		float(mBumpBox[GetSpriteSize(mAnimID)].h)
		) / TILE_HEIGHT;
	
	//test two central-ish positions
	//calc one quater
	float qtr = float(mBumpBox[GetSpriteSize(mAnimID)].w / 4);

	//calc left side
	int ltPos = (int)(
		mPosition.x + 
		mBumpBox[GetSpriteSize(mAnimID)].x + qtr) / TILE_WIDTH;

	//calc right side
	int rtPos = (int)(
		mPosition.x + 
		mBumpBox[GetSpriteSize(mAnimID)].x + 
		(mBumpBox[GetSpriteSize(mAnimID)].w - qtr)) / TILE_WIDTH;

	//store result as booleans
	bool ltOnFloor = mCurrentLevelMap->GetTileType(footPosition, ltPos) != '+';
	bool rtOnFloor = mCurrentLevelMap->GetTileType(footPosition, rtPos) != '+';

	//return result of final test
	return ltOnFloor || rtOnFloor;

	/*
	return !(Collisions::Instance()->Box(
		mCurrentLevelMap->GetTileRect(footPosition, ltPos),
		GetCollisionBox()));
	*/


}

void Character::Jump() {
	if(mCanJump) {
	//if (!mJumping) {
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

void Character::MoveLeft(float deltaTime) {
	if (!HitBlock(-mSpeed)) {
		mPosition.x -= mSpeed;
	}
	mFacingDirection = FACING::FACING_LEFT;
}

void Character::MoveRight(float deltaTime) {
	if (!HitBlock(+mSpeed)) {
		mPosition.x += mSpeed;
	}
	mFacingDirection = FACING::FACING_RIGHT;
}

bool Character::HitBlock(float nextX) {
	return (Collisions::Instance()->Box(
		mCurrentLevelMap->GetTileRect(
			int(mPosition.y / TILE_HEIGHT),
			int(mPosition.x + mBumpBox[GetSpriteSize(mAnimID)].w + nextX)),
		GetCollisionBox()));
}

void Character::RenderSprite(SDL_RendererFlip flip) {
	Anime tmpAnime = GetSpriteAnime();
	string tmpSize = GetSpriteSize(mAnimID);

	SDL_Rect srcRect = {
			tmpAnime.pos.x + ((mSpriteSize[tmpSize].x + 2) * mCurrentFrame),
			tmpAnime.pos.y,
			mSpriteSize[tmpSize].x,
			mSpriteSize[tmpSize].y
	};

	SDL_Rect destRect = GetSpriteRect();
	/*
	SDL_Rect destRect = {
		(int)mPosition.x, 
		(int)mPosition.y,
		mSpriteSize[tmpSize].x,
		mSpriteSize[tmpSize].y
	};
	*/

	Character::Render(srcRect, destRect, flip);
}

void Character::Render(SDL_Rect &srcRect, SDL_Rect &destRect, SDL_RendererFlip &flip) {
	mTexture->RenderRect(srcRect, destRect, flip);
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
	UpdateJump(deltaTime, e);

	if (mMovingLeft) {
		MoveLeft(deltaTime);
	} else if (mMovingRight) {
		MoveRight(deltaTime);
	}

	UpdateAnim(deltaTime, e);
}

//Update Animations
void Character::UpdateAnim(float deltaTime, SDL_Event e) {
	//Change frame?
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f) {
		//Reset frame delay count.
		mFrameDelay = mMaxFrameDelay;

		//Move frame on.
		if (mFrameForward) {
			mCurrentFrame++;
		} else {
			mCurrentFrame--;
		}

		//Loop frame around if it goes beyond the number of frames.
		if (mAnimLoop) {
			if (mCurrentFrame >= mMaxFrames)
				mCurrentFrame = 0;
		} else {
			if (mCurrentFrame >= mMaxFrames - 1) {
				mCurrentFrame = mMaxFrames - 1;
				mFrameForward = false;
			} else if (mCurrentFrame < 0) {
				mCurrentFrame = 0;
				mFrameForward = true;
			}
		}
	}
}

void Character::UpdateJump(float deltaTime, SDL_Event e) {
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

		if (!IsOnFloor()) {
			AddGravity(deltaTime);
		} else {
			//Collided with ground so we can jump again.
			mCanJump = true;
		}
	}

}

void Character::UpdateSpriteInfo(string anim_id) {
	//store sprite id
	mAnimID = anim_id;

	//store number of frames
	Anime tmpAnime = GetSpriteAnime();
	mMaxFrames = tmpAnime.frames;
}