#include "CharacterMario.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition)
	: Character(renderer, imagePath, startPosition) {
}

CharacterMario::~CharacterMario()
{
}

void CharacterMario::Update(float deltaTime, SDL_Event e) {
	Character::Update(deltaTime, e);

	//Handle any events.
	switch (e.type) {
	case SDL_KEYDOWN:
		//Check which key was pressed.
		switch (e.key.keysym.sym) {
		case SDLK_a:
			mMovingLeft = true;
			mMovingRight = false;
			break;

		case SDLK_d:
			mMovingRight = true;
			mMovingLeft = false;
			break;

		case SDLK_w:
			Jump();
			break;
		}
		break;

	case SDL_KEYUP:
		//Check which key was released.
		switch (e.key.keysym.sym) {

		case SDLK_a:
			mMovingLeft = false;
			break;

		case SDLK_d:
			mMovingRight = false;
			break;
		}
		break;
	}
}
