#ifndef COMMONS_H
#define COMMONS_H

#include <string>

/* ENUMERATORS */

enum class SCREENS {
	SCREEN_NULL = -1,
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum class FACING {
	FACING_LEFT = 0,
	FACING_RIGHT
};

/* STRUCTURES */

struct Vector2D {
	float x, y;

	Vector2D() {
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float newX, float newY) {
		x = newX;
		y = newY;
	}
};

struct Point2D {
	int x = 0, y = 0;
};

//use built-in SDL_Rect instead
struct Rect2D {
	float x, y, w, h;

	Rect2D() {
		x = 0; y = 0;
		w = 0; h = 0;
	}

	Rect2D(float newX, float newY, float newW, float newH) {
		x = newX; y = newY;
		w = newW; h = newH;
	}
};

struct Circle2D {
	double x, y, radius;

	Circle2D() {
		x = 0; y = 0;
		radius = 0;
	}

	Circle2D(double newX, double newY, double newRadius) {
		x = newX; y = newY;
		radius = newRadius;
	}
};

struct Colour {
	int cRed = 0, cGreen = 0, cBlue = 0;
};

struct Anime {
	//std::string size_id;		//size id from lookup table/array
	unsigned int frames;		//number of frames
	Point2D pos = Point2D();	//position of first frame on texture
};

struct Option {
	SDL_Rect rect = { 0 };
	std::string text = "";
	bool onHover = false;
	SCREENS screen = SCREENS::SCREEN_NULL;
};

#endif // COMMONS_H
