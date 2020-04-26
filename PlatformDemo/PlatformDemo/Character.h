#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <iostream>

#include "Commons.h"
#include "LevelMap.h"

using namespace std;

class Texture2D;

class Character {
protected:
	SDL_Renderer*	mRenderer;
	Vector2D		mPosition;
	Texture2D*		mTexture;

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

	void			AddGravity (float deltaTime);

public:
	bool			mMovingLeft;
	bool			mMovingRight;

	Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void	Jump();
	virtual void	Render(SDL_RendererFlip flip);
	void			Render(SDL_Rect* srcRect, SDL_Rect* destRect, SDL_RendererFlip flip);
	virtual void	Update(float deltaTime, SDL_Event e);

	void			CancelJump();
	Rect2D			GetCollisionBox();
	Circle2D		GetCollisionCircle();
	float			GetCollisionRadius();
	FACING			GetFacingDirection();
	Vector2D		GetPosition();
	int				GetSpriteWidth();
	int				GetSpriteHeight();
	bool			IsJumping() { return mJumping; }

	void			SetMovementSpeed(float newSpeed = MOVEMENT_SPEED) { mSpeed = newSpeed; }
	void			SetPosition(Vector2D newPosition);
	void			SetFacingDirection(FACING newDirection);
};

#endif // CHARACTER_H