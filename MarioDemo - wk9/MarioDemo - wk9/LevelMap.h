#ifndef LEVELMAP_H
#define LEVELMAP_H

#include "Constants.h"

class LevelMap {
public:
	LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	~LevelMap();

	void ChangeTileAt(unsigned int row, unsigned int column, unsigned int newValue);
	int GetTileAt(unsigned int h, unsigned int w);

private:
	int** mMap;
};

#endif // !LEVELMAP_H