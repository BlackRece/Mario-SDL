#ifndef CHARACTERCOIN_H
#define CHARACTERCOIN_H

#include "Character.h"

class CharacterCoin :
	public Character {
private:
	string mAction;
	string mSize;

	bool mIsAlive;

public:
	CharacterCoin(SDL_Renderer* renderer, string imagePath, LevelMap* map,
		Vector2D startPosition);

	unsigned int	mValue;

	void			Jump();

	void			Render();
	void			Update(float deltaTime, SDL_Event e) override;

	bool			IsAlive() { return mIsAlive; }
	void			SetAlive(bool living) { mIsAlive = living; }
};

#endif // !CHARACTERCOIN_H