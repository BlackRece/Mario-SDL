#ifndef CHARACTERMARIO_H
#define CHARACTERMARIO_H

#include "Character.h"

#include "Commons.h"

class CharacterMario :
	public Character {

private:
	string mAction;
	string mSize;

public:
	CharacterMario(SDL_Renderer* renderer, 
		string imagePath, Vector2D startPosition, LevelMap* map, Colour alphaCol);
	~CharacterMario();

	void	Render();
	void	Respawn(Vector2D startPos);
	void	Update(float deltaTime, SDL_Event e) override;
};

#endif // !CHARACTERMARIO_H