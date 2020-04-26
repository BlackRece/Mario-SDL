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

	void		ChangeTileType(unsigned int row, unsigned int col, char newValue = '+');

	void		LoadMaps();
	bool		LoadMap(std::map<int, char*>&tMap, std::string mapPath);
	
	bool		GetAllTiles(char tileID, std::vector<Point2D>& posList);
	Point2D		GetTextureXY(char tile);
	SDL_Rect	GetTileRect(unsigned int row, unsigned int col);
	char		GetTileType(unsigned int row, unsigned int col);

	void		Render(std::map<int, char*> &tMap);
	void		RenderBackground(float bgYpos) { bgShakePos = bgYpos; Render(mBackMap); }
	void		RenderForeground() { Render(mForeMap); }
	void		RenderPlayground() { Render(mPlayMap); }
	void		RenderPlate(SDL_Rect plateRect);
};

#endif // !LEVELMAP_H