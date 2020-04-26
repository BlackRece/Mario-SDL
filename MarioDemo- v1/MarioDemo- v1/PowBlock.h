#ifndef POWBLOCK_H
#define POWBLOCK_H

/* std lib */
#include <vector>

/* sdl lib */
#include <SDL.h>

/* usr lib */
#include "LevelMap.h"
#include "Texture2D.h"
#include "Commons.h"
#include "Constants.h"

class PowBlock {
private:
	SDL_Renderer*	mRenderer;
	std::vector<Point2D>		mPositions;
	Texture2D*		mTexture;
	LevelMap*		mLevelMap;

	int				mSingleSpriteWidth;
	int				mSingleSpriteHeight;

	int				mNumberOfHitsLeft;
	float			mTimer;

public:
	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock();

	SDL_Rect GetCollisionBox(int index);
	bool IsAvailable() { return mNumberOfHitsLeft > 0; }
	int GetAmount() { return mPositions.size(); }
	void Render();
	void ResetHits();
	void TakeAHit();
	void Update(float deltaTime);
};

#endif // !POWBLOCK_H