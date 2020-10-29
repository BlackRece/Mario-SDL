#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <map>

#include <SDL.h>

#include "Commons.h"
#include "LevelMap.h"

using namespace std;

class Texture2D;

class Character {
protected:
	SDL_Renderer*	mRenderer;
	Vector2D		mPosition;

	Texture2D*		mTexture;
	std::map<string, Anime>		mAnimInfo;		//number of frames and first frame pos
	std::map<string, Point2D>	mSpriteSize;	//size of single sprite frame
	std::map<string, SDL_Rect>	mBumpBox;		//bounding box size and offset

	bool			mIsShooting;

	bool			mJumping;
	bool			mCanJump;
	float			mJumpForce;
	float			mCollisionRadius;

	virtual void	MoveLeft(float deltaTime);
	virtual void	MoveRight(float deltaTime);


private:
	float			mSpeed;
	FACING			mFacingDirection;
	LevelMap*		mCurrentLevelMap;

	string			mAnimID;
	float			mFrameDelay;
	int				mCurrentFrame;
	bool			mFrameForward;

	//int				mSpriteWidth, mSpriteHeight;

	void			AddGravity (float deltaTime);

public:
	float			mMaxFrameDelay;
	int				mMaxFrames;
	bool			mAnimLoop;

	bool			mMovingLeft;
	bool			mMovingRight;

	Character(SDL_Renderer* renderer,
		string imagePath, Vector2D startPosition, 
		LevelMap* map, Colour alphaCol = { 0,0xFF,0xFF });
	~Character();

	void			CancelJump();

	SDL_Rect		virtual GetCollisionBox();
	Circle2D		GetCollisionCircle();
	float			virtual GetCollisionRadius();
	FACING			GetFacingDirection();
	Vector2D		GetPosition();
	
	Anime			GetSpriteAnime();
	int				GetSpriteFrames();
	SDL_Rect		GetSpriteRect();
	string			GetSpriteSize(string size_id);

	bool			HitBlock(float nextX);
	bool			IsJumping() { return mJumping; }
	bool			IsOnFloor();
	virtual void	Jump();

	void			Render(SDL_Rect& srcRect, SDL_Rect& destRect, SDL_RendererFlip& flip);
	void			RenderSprite(SDL_RendererFlip flip, float offsetX = 0.0f);

	void			SetMovementSpeed(float newSpeed = MOVEMENT_SPEED) { mSpeed = newSpeed; }
	void			SetPosition(Vector2D newPosition);
	void			SetFacingDirection(FACING newDirection);
	
	virtual void	Update(float deltaTime, SDL_Event e);
	void			UpdateAnim(float deltaTime, SDL_Event e);
	void			UpdateJump(float deltaTime, SDL_Event e);
	void			UpdateSpriteInfo(string anim_id);

};

#endif // CHARACTER_H