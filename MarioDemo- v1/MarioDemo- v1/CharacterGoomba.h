#ifndef CHARACTERGOOMBA_H
#define CHARACTERGOOMBA_H

#include "Character.h"

class CharacterGoomba :
	public Character {
private:
	float mSingleSpriteWidth, mSingleSpriteHeight;

	bool mIsAlive;
	bool mInjured;
	float mInjuredTime;

	void FlipRightWayUp();

public:

	CharacterGoomba(SDL_Renderer* renderer, string imagePath, LevelMap* map,
		Vector2D startPosition, FACING startFacing, float movementSpeed);

	unsigned int mValue;

	void TakeDamage();
	void Jump();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	bool IsAlive() { return mIsAlive; }
	bool IsInjured() { return mInjured; }
	void SetAlive(bool living) { mIsAlive = living; }
};


#endif // !CHARACTERGOOMBA_H