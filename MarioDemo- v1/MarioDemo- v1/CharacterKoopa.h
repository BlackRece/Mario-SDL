#ifndef CHARACTERKOOPA_H
#define CHARACTERKOOPA_H

#include "Character.h"

#include "Commons.h"

class CharacterKoopa :
	public Character {

private:
	string			mSize, mAction;

	bool			mIsAlive;
	bool			mInjured;
	float			mInjuredTime;

	void			FlipRightWayUp();

public:
	
	CharacterKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map,
		Vector2D startPosition, FACING startFacing, float movementSpeed, 
		Colour alphaCol ={ 169, 203, 197 });

	bool			mIsFlying;
	unsigned int	mValue;

	void			TakeDamage();
	void			Bounce();
	void			Jump();

	void			Render(float offsetX = 0.0f);
	void			Update(float deltaTime, SDL_Event e);

	bool			IsAlive() { return mIsAlive; }
	bool			IsInjured() { return mInjured; }
	void			SetAlive(bool living) { mIsAlive = living; }
};

#endif // !CHARACTERKOOPA_H