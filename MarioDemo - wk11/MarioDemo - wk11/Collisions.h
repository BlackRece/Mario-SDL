#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Commons.h"

class Character;

class Collisions {
private:
	static Collisions* mInstance;
	
	Collisions();

public:
	static Collisions* Instance();

	~Collisions();

	bool Circle(Character* character1, Character* character2);
	bool Circle(Circle2D circle1, Circle2D circle2);
	bool Box(Rect2D rect1, Rect2D rect2);
	bool Pixel(Vector2D point1, Vector2D point2);

};


#endif // !COLLISIONS_H