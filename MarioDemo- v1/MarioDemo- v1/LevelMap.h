#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <string.h>
#include <vector>
#include <map>

#include <SDL.h>

#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"

class LevelMap {
private:
	SDL_Renderer*			mRenderer;
	Texture2D*				mTexture;
	float					bgShakePos;
	std::map<int, char*>	mBackMap, mForeMap, mPlayMap; 

public:
				LevelMap(SDL_Renderer* renderer);
				~LevelMap();

	void		AddMapCol(char ground, char*& newCol);
	void		ChangeTileType(unsigned int row, unsigned int col, char newValue = '+');

	void		LoadMaps();
	bool		LoadMap(std::map<int, char*>&tMap, std::string mapPath);
	
	bool		GetAllTiles(char tileID, std::vector<Point2D>& posList);
	bool		GetMapCol(char ground, int index, char*& col);
	Point2D		GetTextureXY(char tile);
	SDL_Rect	GetTileRect(unsigned int row, unsigned int col);
	char		GetTileType(unsigned int row, unsigned int col);

	void		Render(std::map<int, char*> &tMap, float offsetX = 0.0f);
	void		RenderBackground(float offsetX = 0.0f, float bgYpos = 0.0f) 
					{ bgShakePos = bgYpos; Render(mBackMap, offsetX); }
	void		RenderForeground(float offsetX = 0.0f) { Render(mForeMap, offsetX); }
	void		RenderPlayground(float offsetX = 0.0f) { Render(mPlayMap, offsetX); }
	void		RenderPlate(SDL_Rect plateRect);
};

#endif // !LEVELMAP_H