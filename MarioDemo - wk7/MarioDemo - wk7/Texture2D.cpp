#include "Texture2D.h"

/* sdl lib */
#include <SDL_image.h>

Texture2D::Texture2D(SDL_Renderer* renderer) {
	mRenderer = renderer;
	mTexture = nullptr;

	mWidth = 0;
	mHeight = 0;
}

Texture2D::~Texture2D() {
	//Free up the memory.
	Free();

	mRenderer = nullptr;
}

bool Texture2D::LoadFromFile(std::string path) {
	//Remove the memory used for a previous texture.
	Free();

	//Load the image.
	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface == nullptr) {
		std::cout << "Unable to load image as surface. Error: " << IMG_GetError() << std::endl;
	} else {
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));

		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		if (mTexture == nullptr) {
			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;
		} else {
			//Set the dimensions.
			mWidth	= pSurface->w;
			mHeight = pSurface->h;
		}

		//Remove the loaded surface now that we have the texture.
		SDL_FreeSurface(pSurface);
	}

	//Return whether the process was successful.
	return mTexture != nullptr;
}

void Texture2D::Free() {
	//Check if the texture exists before removing it.
	if (mTexture != nullptr) {
		SDL_DestroyTexture(mTexture);
	}

	mWidth = 0;
	mHeight = 0;
	mTexture = nullptr;
}

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle) {
	//Set where to render the texture.
	SDL_Rect renderLocation = {
		newPosition.x, newPosition.y,
		mWidth, mHeight
	};

	//render to screen
	SDL_RenderCopyEx(mRenderer, mTexture, nullptr, &renderLocation, 0, nullptr, SDL_FLIP_NONE);

}
