#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <iostream>

#include "Commons.h"

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

	void			AddGravity (float deltaTime);

public:
	bool			mMovingLeft;
	bool			mMovingRight;

	Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition);
	~Character();

	virtual void	Render(SDL_RendererFlip flip);
	virtual void	Update(float deltaTime, SDL_Event e);

	Vector2D		GetPosition();
	float			GetCollisionRadius();
	Circle2D		GetCollisionCircle();
	Rect2D			GetCollisionBox();

	void			Jump();
	void			SetPosition(Vector2D newPosition);
	void			SetFacingDirection(FACING newDirection);
	FACING			GetFacingDirection();
};

#endif // CHARACTER_H