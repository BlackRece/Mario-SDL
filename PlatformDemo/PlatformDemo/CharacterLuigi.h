#ifndef CHARACTERLUIGI_H
#define CHARACTERLUIGI_H

#include "Character.h"
class CharacterLuigi :
	public Character {
private:
	int		mSpriteWidth, mSpriteHeight;

	float mFrameDelay;
	int mCurrentFrame;
	int mFrameCount_Walk;
	int mFrameCount_Idle;
	int mFrameCount_Fire;
	int mFrameCount_FireIdle;
	int mFrameCount_Jump;

	bool mMoving;

	SDL_Rect* mSpriteRect;	//source rect - position on sprite sheet
	SDL_Rect* mScreenRect;	//destination rect - position on screen

public:
	CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterLuigi();

	void	Render();
	void	SetState(bool newState);
	void	Update(float deltaTime, SDL_Event e);
};

#endif // !CHARACTERLUIGI_H