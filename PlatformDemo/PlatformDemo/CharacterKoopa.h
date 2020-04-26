#ifndef CHARACTERKOOPA_H
#define CHARACTERKOOPA_H

#include "Character.h"

class CharacterKoopa :
	public Character {
private:
	int mSingleSpriteWidth, mSingleSpriteHeight;

	bool mIsAlive;
	bool mInjured;
	float mInjuredTime;

	void FlipRightWayUp();

public:
	
	CharacterKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map,
		Vector2D startPosition, FACING startFacing, float movementSpeed);

	void TakeDamage();
	void Jump();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	bool IsAlive() { return mIsAlive; }
	bool IsInjured() { return mInjured; }
	void SetAlive(bool living) { mIsAlive = living; }
};

#endif // !CHARACTERKOOPA_H