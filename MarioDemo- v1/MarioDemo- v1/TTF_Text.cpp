#include "TTF_Text.h"
#include <iostream>

TTF_Text::TTF_Text() {
    mRenderer = nullptr;

    mGlyph.clear();

    mFont = nullptr;
}

bool TTF_Text::LoadFont(SDL_Renderer* newRenderer, std::string newFontPath, int newFontSize) {
    bool success = true;

    mRenderer = newRenderer;

    if (!mGlyph.empty()) {
        mGlyph.clear();
    }
    
    mFont = TTF_OpenFont(newFontPath.c_str(), newFontSize);
    if (mFont == nullptr) {
        std::cout << "\nFailed to load " << newFontPath << " font!" <<
            "\nSDL_ttf Error: " << TTF_GetError() << std::endl;
        success = false;
    } else {
        for (char i = 32; i < 127; i++) {
            std::pair<char, TTF_Char> tmpGlyph = std::make_pair(i, TTF_Char());

            if (LoadRenderedChar(i, tmpGlyph.second)) {
                mGlyph.insert(tmpGlyph);
            } else {
                std::cout << "\nTTF_Text Error: " << i << " not loaded." << std::endl;
            }
        }
    }

    return success;
}

TTF_Text::~TTF_Text() {
    if (!mGlyph.empty()) {
        for (std::map<char, TTF_Char>::iterator i = mGlyph.begin(); i != mGlyph.end(); i++) {
            if (i->second._glyphTexture != nullptr) {
                SDL_DestroyTexture(i->second._glyphTexture);
                i->second._glyphTexture = nullptr;
            }
        }
        mGlyph.clear();
    }

    TTF_CloseFont(mFont);
    mFont = nullptr;
}
/*
bool TTF_Text::LoadFromFile(std::string path) {
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
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
*/
bool TTF_Text::LoadRenderedChar(const char textureChar, TTF_Char& targetChar) {
    //SDL_Texture* resultTexture = nullptr;
    //Clear pre-existing texture data and reset dimensions
    Free(targetChar._glyphTexture);
    targetChar._width = 0;
    targetChar._height = 0;

    //Render text surface (white as default)
    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, &textureChar, { 0xFF,0xFF,0xFF });
    if (textSurface == nullptr) {
        std::cout << "\nUnable to render text surface!" <<
            "\nSDL_ttf Error: " << TTF_GetError() << std::endl;
    } else {
        //Create texture from surface pixels
        targetChar._glyphTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if (targetChar._glyphTexture == nullptr) {
            std::cout << "\nUnable to create texture from rendered text!" <<
                "\nSDL Error: " << SDL_GetError() << std::endl;
        } else {
            //Get image dimensions
            targetChar._width = textSurface->w;
            targetChar._height= textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return targetChar._glyphTexture != nullptr;
}

void TTF_Text::Free(SDL_Texture* targetTexture) {
    if (targetTexture != nullptr) {
        SDL_DestroyTexture(targetTexture);
        targetTexture = nullptr;

    }
}

void TTF_Text::RenderString(SDL_Rect dstRect, std::string theString, SDL_Color theColour) {
    SDL_Rect charRect = dstRect;

    charRect.w = (int)(dstRect.w / theString.length());

    for (int i = 0; i < (int)theString.length(); i++) {
        SDL_SetTextureColorMod(
            mGlyph[theString.at(i)]._glyphTexture, theColour.r, theColour.g, theColour.b
        );
        Render(theString.at(i), charRect);
        
        charRect.x += charRect.w;
    }
}

void TTF_Text::Render(
    char letter, SDL_Rect dstRect, double angle, 
    SDL_Point* center, SDL_RendererFlip flip) {
    if (SDL_RenderCopy(mRenderer, mGlyph[letter]._glyphTexture, nullptr, &dstRect) != 0) {
        std::cout << "\nGlyph RenderRect Error: " << SDL_GetError() << std::endl;
    }
}

/*
void TTF_Text::Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle) {
    SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &destRect, angle, nullptr, flip);
}
*/
