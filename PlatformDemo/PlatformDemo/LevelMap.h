#ifndef LEVELMAP_H
#define LEVELMAP_H

#include "Constants.h"

struct Pixel {
	unsigned short _r, _g, _b;
};

class LevelMap {
public:
	LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	~LevelMap();

	void ChangeTileAt(unsigned int row, unsigned int column, unsigned int newValue);
	int GetTileAt(unsigned int h, unsigned int w);
	/*due to time, abandon loading an image for use as the level map... :(*/
	bool LoadBMP(const char* levelPath);
	void LoadMapSurface(const char* levelPath);

private:
	int** mMap;
	Pixel** mImgMap;
};

#endif // !LEVELMAP_H