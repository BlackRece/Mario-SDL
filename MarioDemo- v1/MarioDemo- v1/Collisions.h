#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <SDL.h>

#include "Commons.h"

class Character;

class Collisions {
private:
	static Collisions* mInstance;
	
	Collisions();

public:
	static Collisions* Instance();

	~Collisions();

	bool Circle(Circle2D circle1, Circle2D circle2);
	bool Box(SDL_Rect rect1, SDL_Rect rect2);
	bool Pixel(Vector2D point1, Vector2D point2);
	bool PointInArea(Point2D point, SDL_Rect area);

};


#endif // !COLLISIONS_H