#include "LevelMap.h"
#include <fstream>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <string>


//-------------------------------------------
// When making a map, remember:
// 0 = Empty Area     1 = Blocked Area
//-------------------------------------------

LevelMap::LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]) {
	//Allocate memory for level map.
	mMap = new int* [MAP_HEIGHT];
	for (unsigned int i = 0; i < MAP_HEIGHT; i++) {
		mMap[i] = new int[MAP_WIDTH];
	}

	//Allocate memory for level's pixel map.
	mImgMap = new Pixel* [16];
	for (int i = 0; i < 16; i++) {
		mImgMap[i] = new Pixel [16];
	}

	/*
	mImgMap = new Pixel* [MAP_HEIGHT];
	for (int i = 0; i < MAP_HEIGHT; i++) {
		mImgMap[i] = new Pixel[MAP_WIDTH];
	}
	*/

	//Populate the array.
	for (unsigned int i = 0; i < MAP_HEIGHT; i++) {
		for (unsigned int j = 0; j < MAP_WIDTH; j++) {
			mMap[i][j] = map[i][j];
		}
	}
}

LevelMap::~LevelMap() {
	//Delete all elements of the array.
	for (unsigned int i = 0; i < MAP_HEIGHT; i++) {
		delete[] mMap[i];
	}
	delete[] mMap;
}

int LevelMap::GetTileAt(unsigned int h, unsigned int w) {
	if (h < MAP_HEIGHT && w < MAP_WIDTH) {
		return mMap[h][w];
	}

	return 0;
}

bool LevelMap::LoadBMP(const char* levelPath) {
	char header[54];
	unsigned char* data;
	int dataPos;
	int width, height;
	int imageSize;
	//Pixel** bmp;

	std::ifstream file;
	file.open(levelPath, std::ios::binary);
	if (!file.good()) return false;

	file.read(header, 54);
	if (header[0] != 'B' || header[1] != 'M') return false;

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	//move file marker to beginning of pixel data
	file.seekg(dataPos);

	//data = new unsigned char[width * 3];
	//adjust buffer size
	data = new unsigned char[imageSize];


	//read whole image
	file.read((char*)data, imageSize);

	for (int row = 0; row < height; row++) {
		//**** reading a line of bmp file returns an empty array ****//
		//read a row of pixels
		//file.read((char*)data, width * 24 * 3);

		if (data != 0x00) {
			for (int col = 0; col < width * 3; col += 3) {

				mImgMap[row][col] = {
					data[col + 2],	// red channel
					data[col + 1],	// green channel
					data[col + 0]	// blue channel
				};
			}
		}
	}

	//DEBUG
	file.seekg(dataPos);
	file.read((char*)data, imageSize);

	file.close();

	//DEBUG
	for (int i = 0; i < imageSize; i+=3) {
	//for (int i = 0; i < 320; i += 3) {
		mImgMap[i] = new Pixel({
			data[i+2], data[i+1], data[i]
		});

		std::cout << "{" <<
			std::hex << (unsigned short)data[i+2] << "-" <<
			std::hex << (unsigned short)data[i+1] << "-" <<
			std::hex << (unsigned short)data[i] <<
			"}";
		if (i % width == 0) {
			std::cout << std::endl;
		} else {
			std::cout << ", ";
		}
	}

	return true;
}

void LevelMap::LoadMapSurface(const char* levelPath) {
	/*
	create/load map image
	create surface from image
	read pixel data from surface->pixel into level map array
	return level map array

	*/

	SDL_PixelFormat* fmt;
	SDL_Color* color;
	Uint8 index;

	SDL_Surface* pMap = IMG_Load(levelPath);
	if (pMap == nullptr) {
		std::cout << "Unable to load map image. Error: " << IMG_GetError() << std::endl;
		return;
	}

	fmt = pMap->format;


	/* Check the bitdepth of the surface */
	if (fmt->BitsPerPixel != 8) {
		std::cout << "Not an 8 - bit surface." << std::endl;
		return;
	}

	// state of surface
	int pState = SDL_SetSurfaceRLE(pMap, SDL_MUSTLOCK(pMap));

	// lock surface if surface needs locking
	if (pState != 0) SDL_LockSurface(pMap);

	for (int i = 0; i < pMap->pitch * pMap->h; i++) {
		/* Get the topleft pixel */
		// get pixel index
		index = *(Uint8*)pMap->pixels[i];

		// get pixel data from palette
		color = &fmt->palette->colors[index];

		//DEBUG: show result
		std::cout << "i = " << std::to_string(i) << ": " <<
			std::to_string(color->r) << ", " <<
			std::to_string(color->g) << ", " <<
			std::to_string(color->b) << std::endl;
	}
	
	// unlock surface if surface needing locking
	if (pState != 0) SDL_UnlockSurface(pMap);

	//dispose of surface
	SDL_FreeSurface(pMap);
}

void LevelMap::ChangeTileAt(unsigned int row, unsigned int column, unsigned int newValue) {
	if (row < MAP_HEIGHT && column < MAP_WIDTH) {
		mMap[row][column] = newValue;
	}
}