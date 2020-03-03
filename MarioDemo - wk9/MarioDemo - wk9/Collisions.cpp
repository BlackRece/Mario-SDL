#include "Collisions.h"
#include "Character.h"

Collisions* Collisions::mInstance = nullptr;

Collisions::Collisions() {
}

Collisions* Collisions::Instance() {
	if (!mInstance) {
		mInstance = new Collisions;
	}

	return mInstance;
}

Collisions::~Collisions() {
	mInstance = nullptr;
}

bool Collisions::Circle(Character* character1, Character* character2) {
	Vector2D vec = Vector2D(
		(character1->GetPosition().x - character2->GetPosition().x),
		(character1->GetPosition().y - character2->GetPosition().y)
	);

	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));

	double combineDistance = double(
		character1->GetCollisionRadius() + character2->GetCollisionRadius()
		);

	return distance < combineDistance;
}

bool Collisions::Circle(Circle2D circle1, Circle2D circle2) {
	Vector2D vec = Vector2D(
		(circle1.x - circle2.x),
		(circle1.y - circle2.y)
	);

	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));

	double combineDistance = double( circle1.radius + circle2.radius );

	return distance < combineDistance;
}

bool Collisions::Box(Rect2D rect1, Rect2D rect2) {
	if (
		rect1.x + (rect1.w / 2) > rect2.x &&
		rect1.x + (rect1.w / 2) < rect2.x+rect2.w &&

		rect1.y + (rect1.h / 2) > rect2.y &&
		rect1.y + (rect1.h / 2) > rect2.y+rect2.h
		) {
		return true;
	}
	return false;
}

bool Collisions::Pixel(Vector2D point1, Vector2D point2) {
	return (point1.x == point2.x) && (point1.y == point2.y);
}
