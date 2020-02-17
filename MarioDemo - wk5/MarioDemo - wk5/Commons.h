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

#endif // COMMONS_H
