#ifndef STRINGRENDER_H
#define STRINGRENDER_H

#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Commons.h"
#include "Constants.h"

class StringRender {
public:
    //Initializes variables
    StringRender();
    StringRender(SDL_Renderer* renderer);

    void LoadFont(SDL_Renderer* renderer, std::string fontPath = FONT_PATH, int fontSize = 36);
        
    //Deallocates memory
    ~StringRender();

    //Creates image from font string
    bool LoadRenderedString(const char* charString, SDL_Color textColour);

    //Deallocates texture
    void Free();

    void Render(SDL_Rect& dstRect);

    //Gets image dimensions
    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }

private:
    TTF_Font* mFont;
    SDL_Renderer* mRenderer;
    SDL_Texture* mTexture;

    int mWidth, mHeight;

};

#endif // !STRINGRENDER_H