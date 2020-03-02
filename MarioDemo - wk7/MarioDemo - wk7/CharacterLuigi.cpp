#include "CharacterLuigi.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition)
	: Character(renderer, imagePath, startPosition) {
}

CharacterLuigi::~CharacterLuigi()
{
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e) {
	Character::Update(deltaTime, e);

	//Handle any events.
	switch (e.type) {
	case SDL_KEYDOWN:
		//Check which key was pressed.
		switch (e.key.keysym.sym) {
		case SDLK_LEFT:
			mMovingLeft = true;
			mMovingRight = false;
			break;

		case SDLK_RIGHT:
			mMovingRight = true;
			mMovingLeft = false;
			break;

		case SDLK_UP:
			Jump();
			break;
		}
		break;

	case SDL_KEYUP:
		//Check which key was released.
		switch (e.key.keysym.sym) {

		case SDLK_LEFT:
			mMovingLeft = false;
			break;

		case SDLK_RIGHT:
			mMovingRight = false;
			break;
		}
		break;
	}
}
