#ifndef COMMONS_H
#define COMMONS_H

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
	float x, y, radius;

	Circle2D() {
		x = 0; y = 0;
		radius = 0;
	}

	Circle2D(float newX, float newY, float newRadius) {
		x = newX; y = newY;
		radius = newRadius;
	}
};

enum class SCREENS {
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



#endif // COMMONS_H
