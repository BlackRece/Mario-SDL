#ifndef CHARACTERLUIGI_H
#define CHARACTERLUIGI_H

#include "Character.h"
class CharacterLuigi :
	public Character {
public:
	CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition);
	~CharacterLuigi();

	void	Update(float deltaTime, SDL_Event e);
};

#endif // !CHARACTERLUIGI_H