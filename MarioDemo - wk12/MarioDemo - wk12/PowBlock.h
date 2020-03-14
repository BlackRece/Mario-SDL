#ifndef POWBLOCK_H
#define POWBLOCK_H

/* sdl lib */
#include <SDL.h>

/* usr lib */
#include "LevelMap.h"
#include "Texture2D.h"
#include "Commons.h"

class PowBlock {
private:
	SDL_Renderer*	mRenderer;
	Vector2D		mPosition;
	Texture2D*		mTexture;
	LevelMap*		mLevelMap;

	float			mSingleSpriteWidth;
	float			mSingleSpriteHeight;

	int				mNumberOfHitsLeft;

public:
	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock();

	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight()); };
	bool IsAvailable() { return mNumberOfHitsLeft > 0; }
	void Render();
	void ResetHits();
	void TakeAHit();


};

#endif // !POWBLOCK_H