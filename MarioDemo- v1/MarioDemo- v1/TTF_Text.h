#ifndef TTF_TEXT_H
#define TTF_TEXT_H

#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Commons.h"
#include "Constants.h"

struct TTF_Char {
	int _width, _height;
	SDL_Texture* _glyphTexture;
};

class TTF_Text {
public:
    //Initializes variables
    TTF_Text();

    //Deallocates memory
    ~TTF_Text();

    //Loads image at specified path
    //bool LoadFromFile(std::string path);
    bool LoadFont(
        SDL_Renderer* newRenderer,
        std::string newFontPath = FONT_PATH,
        int newFontSize = 28
    );

    //Creates image from font string
    bool LoadRenderedChar(char textureChar, TTF_Char& targetChar);

    //Deallocates texture
    void Free(SDL_Texture* targetTexture);

    //Renders a string at a given vector in a given colour (default = white colour)
    void RenderString(SDL_Rect dstRect, std::string theString, SDL_Color theColour = { 1,1,1 });

    //Renders texture at given point
    void Render(
        char letter, SDL_Rect dstRect,
        double angle = 0.0, SDL_Point* center = NULL,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    );
    
    //Gets image dimensions
    int GetWidth(const char i) { return mGlyph[i]._width; }
    int GetHeight(const char i) { return mGlyph[i]._height; }

private:
    TTF_Font* mFont;
    SDL_Renderer* mRenderer;

    std::map<char, TTF_Char> mGlyph;
};

#endif // !TTF_TEXT_H