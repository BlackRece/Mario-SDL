#ifndef CHARACTERMARIO_H
#define CHARACTERMARIO_H

#include "Character.h"

class CharacterMario :
	public Character {
public:
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterMario();

	void	Render();
	void	Update(float deltaTime, SDL_Event e);
};

#endif // !CHARACTERMARIO_H