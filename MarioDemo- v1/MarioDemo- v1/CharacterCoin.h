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
	unsigned int	mValue;

	CharacterCoin(SDL_Renderer* renderer, string imagePath, LevelMap* map,
		Vector2D startPosition);

	bool			IsAlive() { return mIsAlive; }
	
	void			Jump();

	void			Render(float offsetX = 0.0f);
	
	void			SetAlive(bool living) { mIsAlive = living; }
	
	void			Update(float deltaTime, SDL_Event e) override;

};

#endif // !CHARACTERCOIN_H