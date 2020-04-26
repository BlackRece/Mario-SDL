#include "StringRender.h"
#include <string>

StringRender::StringRender() {
    mRenderer = nullptr;
    mTexture = nullptr;
    mFont = nullptr;
    mWidth = 0; mHeight = 0;
}

void StringRender::LoadFont(SDL_Renderer* renderer, std::string fontPath, int fontSize) {
    mRenderer = renderer;
    
    mFont = TTF_OpenFont((char*)fontPath.c_str(), 36);
    if (mFont == nullptr) {
        std::cout << "\nFailed to load " << FONT_PATH << " font!" <<
            "\nSDL_ttf Error: " << TTF_GetError() << std::endl;
    }
}

StringRender::~StringRender() {
    Free();

    TTF_CloseFont(mFont);
    mFont = nullptr;
}

bool StringRender::LoadRenderedString(const char* charString, SDL_Color textColour) {
    //Get rid of preexisting texture
    Free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, charString, textColour);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if (mTexture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return mTexture != NULL;
}

void StringRender::Free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void StringRender::Render(SDL_Rect& dstRect) {
    SDL_RenderCopy(mRenderer, mTexture, nullptr, &dstRect);
}