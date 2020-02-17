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

enum class SCREENS {
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

#endif // COMMONS_H
