#ifndef CHARACTERSPINY_H
#define CHARACTERSPINY_H

#include "Character.h"

#include "Commons.h"

class CharacterSpiny :
	public Character {

private:
	string			mSize, mAction;

	bool			mIsAlive;
	bool			mInjured;
	float			mInjuredTime;

	void			FlipRightWayUp();

public:

	unsigned int	mValue;

					CharacterSpiny(SDL_Renderer* renderer, string imagePath, LevelMap* map,
						Vector2D startPosition, FACING startFacing, float movementSpeed,
						Colour alphaCol = { 169, 203, 197 });
					~CharacterSpiny() {};

	bool			IsAlive() { return mIsAlive; }
	bool			IsInjured() { return mInjured; }

	void			Jump();

	void			Render(float offsetX = 0.0f);

	void			SetAlive(bool living) { mIsAlive = living; }
	void			TakeDamage();

	void			Update(float deltaTime, SDL_Event e);

};

#endif // !CHARACTERSPINY_H