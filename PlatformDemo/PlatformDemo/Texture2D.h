#ifndef TEXTURE2D_H
#define TEXTURE2D_H

/* std lib*/
#include <iostream>

/* sdl lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"

class Texture2D {
public:
	Texture2D(SDL_Renderer* renderer);
	~Texture2D();

	bool			LoadFromFile(std::string path);
	void			Free();
	void			Render(Vector2D newPosition, SDL_RendererFlip flip, double angle = 0.0f);
	void			Render(SDL_Rect* srcRect, SDL_Rect *destRect, SDL_RendererFlip flip, double angle = 0.0);

	int				GetWidth()		{ return mWidth; }
	int				GetHeight() { return mHeight; }
private:
	SDL_Renderer*	mRenderer;
	SDL_Texture*	mTexture;

	int				mWidth;
	int				mHeight;
};

#endif // TEXTURE2D_H
