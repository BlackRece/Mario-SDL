#ifndef CHARACTERLUIGI_H
#define CHARACTERLUIGI_H

#include "Character.h"

#include "Commons.h"

class CharacterLuigi :
	public Character {
	
private:
	string mAction;
	string mSize;
	
public:
	CharacterLuigi(SDL_Renderer* renderer, 
		string imagePath, Vector2D startPosition, LevelMap* map, Colour alphaCol);
	~CharacterLuigi();

	void	Render();
	void	Respawn(Vector2D startPos);
	void	Update(float deltaTime, SDL_Event e);
};

#endif // !CHARACTERLUIGI_H