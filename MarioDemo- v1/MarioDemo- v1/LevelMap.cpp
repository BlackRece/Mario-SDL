#include "LevelMap.h"
#include <fstream>
#include <string>

//-------------------------------------------
// When making a map, remember:
// 0 = Empty Area     1 = Blocked Area
//-------------------------------------------

LevelMap::LevelMap(SDL_Renderer* renderer) {
	mRenderer = renderer;

	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(TERRAIN_PATH)) {
		std::cout << "Failed to load " << TERRAIN_PATH << " texture! :(" << std::endl;
	}

	mBackMap.clear();
	mForeMap.clear();
	mPlayMap.clear();

	LoadMaps();
}

LevelMap::~LevelMap() {
	mRenderer = nullptr;
	
	delete mTexture;
	mTexture = nullptr;
}

void LevelMap::ChangeTileType(unsigned int row, unsigned int col, char newValue) {
	char* targetRow = mPlayMap.at(row);
	targetRow[col] = newValue;
}

bool LevelMap::GetAllTiles(char tileID, std::vector<Point2D>& posList) {

	bool result = false;
	std::map<int, char*>::iterator tmpIT;

	for (tmpIT = mPlayMap.begin(); tmpIT != mPlayMap.end(); tmpIT++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			//test each char in row array
			if(tmpIT->second[i] == tileID) {
				//if match found
				//set flag
				result = true;

				//add to position array
				posList.push_back({ i, tmpIT->first });
			}
		}
	}

	return result;
}

//returns texture position (from file)
Point2D LevelMap::GetTextureXY(char tile) {
	Point2D result;

	switch (tile) {

		//foreground: bush
	case 'M': result = { 0,  68 }; break;
	case ',': result = { 34,  68 }; break;
	case '.': result = { 68,  68 }; break;

		//foreground: pipe
	case 'Q': result = { 0, 204 }; break;
	case 'W': result = { 34, 204 }; break;
	case 'E': result = { 68, 204 }; break;
	case 'A': result = { 0, 238 }; break;
	case 'S': result = { 34, 238 }; break;
	case 'D': result = { 68, 238 }; break;

		//foreground: plate
	case 'q': result = { 0, 408 }; break;
	case 'w': result = { 34, 408 }; break;
	case 'e': result = { 68, 408 }; break;
	case 'a': result = { 0, 442 }; break;
	case 's': result = { 34, 442 }; break;
	case 'd': result = { 68, 442 }; break;
	case 'z': result = { 0, 476 }; break;
	case 'x': result = { 34, 476 }; break;
	case 'c': result = { 68, 476 }; break;

		//playground: block
	case 'T': result = { 34, 272 }; break;

		//playground: top-brick
	case 'R': result = { 0, 340 }; break;

		//playground: bottom-brick
	case 'F': result = { 0, 374 }; break;

		//background: cloud
	case 'Z': result = { 0,   0 }; break;
	case 'X': result = { 34,   0 }; break;
	case 'C': result = { 68,   0 }; break;
	case 'V': result = { 0,  34 }; break;
	case 'B': result = { 34,  34 }; break;
	case 'N': result = { 68,  34 }; break;

		//background: hill
	case 'L': result = { 34, 102 }; break;
	case 'Y': result = { 0, 136 }; break;
	case 'U': result = { 34, 136 }; break;
	case 'I': result = { 68, 136 }; break;
	case 'H': result = { 0, 170 }; break;
	case 'J': result = { 34, 170 }; break;
	case 'K': result = { 68, 170 }; break;

		//background: blank
	case 'O': result = { 0, 102 }; break;

		//any/all undefined value
	default: result = { -1,-1 }; break;
	}

	return result;
}

SDL_Rect LevelMap::GetTileRect(unsigned int row, unsigned int col) {
	SDL_Rect result = {};
	if (row >= 0 && row < MAP_HEIGHT &&
		col >= 0 && col < MAP_WIDTH) {
		result = {
			int(col * TILE_WIDTH),
			int(row * TILE_HEIGHT),
			int(TILE_WIDTH),
			int(TILE_HEIGHT)
		};
	}
	return result;
}

char LevelMap::GetTileType(unsigned int row, unsigned int col) {
	char result = '+';
	if (row >= 0 && row < MAP_HEIGHT &&
		col >= 0 && col < MAP_WIDTH) {
	/*
		char* targetRow = mPlayMap.at(row);
		result = targetRow[col];
	*/
		result = mPlayMap[row][col];
	}
	return result;

}

bool LevelMap::LoadMap(std::map<int, char*>&tMap, std::string mapPath) {
	std::ifstream fMap(mapPath);
	std::string rawRow;
	int rowCount = 0;
	char* tRow;
	bool result = false;

	if (fMap.good()) {
		//mapArr = new char* [MAP_HEIGHT];
		while (std::getline(fMap, rawRow, '\n')) {
			tRow = new char[MAP_WIDTH];
			for (int i = 0; i < MAP_WIDTH; i++) {
				tRow[i] = rawRow.at(i);
			}
			tMap.insert(std::make_pair(rowCount, tRow));
	
			rowCount++;
			rawRow.clear();
		}
		result = true;
	}

	fMap.close();
	return result;

}

void LevelMap::LoadMaps() {
	LoadMap(mBackMap, BACK_MAP);
	LoadMap(mForeMap, FORE_MAP);
	LoadMap(mPlayMap, PLAY_MAP);
}

void LevelMap::Render(std::map<int, char*> &tMap){
	std::map<int, char*>::iterator tmpIT;
	Point2D tile;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Rect srcRect = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	SDL_Rect destRect;

	for (tmpIT = tMap.begin(); tmpIT != tMap.end(); tmpIT++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			tile = GetTextureXY(tmpIT->second[i]);

			//if valid/visible tile
			if (tile.x != -1 || tile.y != -1) {

				//set texture rect
				srcRect.x = tile.x;
				srcRect.y = tile.y;

				//set screen rect
				destRect = GetTileRect(tmpIT->first, i);
				destRect.y += int(bgShakePos);

				//render tile
				mTexture->RenderRect(srcRect, destRect, flip);
			}
		}
	}
}

void LevelMap::RenderPlate(SDL_Rect plateTextRect) {
	//dimensions of tile
	SDL_Rect srcRect = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	SDL_Rect destRect = { 0, 0, TILE_WIDTH, TILE_HEIGHT };

	Point2D tile;
	Point2D plateDims = {		//dimensions of plate's tile array
		(plateTextRect.w / TILE_WIDTH) + 3,		//number of tiles wide
		(plateTextRect.h / TILE_HEIGHT) + 2		//number of tiles tall
	};

	Point2D plateOffset = {			//dimensions of plate area when drawn
		plateTextRect.x - (((plateDims.x * TILE_WIDTH) - plateTextRect.w) / 2),		//width
		(int)abs(((plateDims.y * TILE_HEIGHT) / 3.5f) - plateTextRect.y)			//height
	};
	
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	char** plateMap;
	plateMap = new char* [plateDims.y];
	for (int row = 0; row < plateDims.y; row++) {
		plateMap[row] = new char[plateDims.x];

		//top row
		if (row == 0) {
			for (int col = 0; col < plateDims.x; col++) {
				if (col == 0) {
					//left corner
					plateMap[row][col] = 'q';
				} else if (col == plateDims.x - 1) {
					//right corner
					plateMap[row][col] = 'e';
				} else {
					//mid
					plateMap[row][col] = 'w';
				}
			}
		}

		//bottom row
		else if (row == plateDims.y - 1) {
			for (int col = 0; col < plateDims.x; col++) {
				if (col == 0) {
					//left corner
					plateMap[row][col] = 'z';
				} else if (col == plateDims.x - 1) {
					//right corner
					plateMap[row][col] = 'c';
				} else {
					//mid
					plateMap[row][col] = 'x';
				}
			}
		}

		//middle row(s)
		else {
			for (int col = 0; col < plateDims.x; col++) {
				if (col == 0) {
					//left side
					plateMap[row][col] = 'a';
				} else if (col == plateDims.x - 1) {
					//right side
					plateMap[row][col] = 'd';
				} else {
					//mid
					plateMap[row][col] = 's';
				}
			}
		}
	}

	for (int row = 0; row < plateDims.y; row++) {
		for (int col = 0; col < plateDims.x; col++) {
			//get tile x and y from texture
			tile = GetTextureXY(plateMap[row][col]);

			//set texture rect
			srcRect.x = tile.x;
			srcRect.y = tile.y;

			//set screen rect
			destRect.x = (col * TILE_WIDTH) + plateOffset.x;
			destRect.y = (row * TILE_HEIGHT) + plateOffset.y;

			//render tile
			mTexture->RenderRect(srcRect, destRect, flip);
		}
	}

}