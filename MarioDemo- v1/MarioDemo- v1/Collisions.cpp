#include "Collisions.h"
#include <iostream>

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

bool Collisions::Circle(Circle2D circle1, Circle2D circle2) {
	double cx = circle1.x - circle2.x;
	double cy = circle1.y - circle2.y;

	double distance = sqrt((cx * cx) + (cy * cy));
	
	double combineDistance = circle1.radius + circle2.radius;

	return distance < combineDistance;
}

bool Collisions::Box(SDL_Rect rect1, SDL_Rect rect2) {
	if (
		rect1.x + (rect1.w / 2) > rect2.x &&
		rect1.x + (rect1.w / 2) < rect2.x+rect2.w &&

		rect1.y + (rect1.h / 2) > rect2.y &&
		rect1.y + (rect1.h / 2) < rect2.y+rect2.h
		) {
		//DEBUG
		/*
		std::cout <<
			"rect1:" <<
			" (x" << std::to_string(rect1.x) <<
			", y" << std::to_string(rect1.y) <<
			", w" << std::to_string(rect1.w) <<
			", h" << std::to_string(rect1.h) <<
			")\nrect2:" <<
			" (x" << std::to_string(rect2.x) <<
			", y" << std::to_string(rect2.y) <<
			", w" << std::to_string(rect2.w) <<
			", h" << std::to_string(rect2.h) <<
			")" << std::endl;

		std::cout <<
			"mid1.x > left2  " <<
			rect1.x << " + (" << rect1.w << " / 2) > " << rect2.x;
		if (rect1.x + (rect1.w / 2) > rect2.x) {
			std::cout << " = True\n";
		} else {
			std::cout << " = False\n";
		}
		std::cout <<
			"mid1.x < right2 " << rect1.x << " + (" << rect1.w << " / 2) > " <<
			rect2.x << " + " << rect2.w;
		if (rect1.x + (rect1.w / 2) > rect2.x + rect2.w) {
			std::cout << " = True\n";
		} else {
			std::cout << " = False\n";
		}

		std::cout <<
			"mid1.y > top2   " << rect1.y << " + (" << rect1.h << " / 2) > " << rect2.y;
		if (rect1.y + (rect1.h / 2) > rect2.y) {
			std::cout << " = True\n";
		} else {
			std::cout << " = False\n";
		}
		std::cout <<
			"mid1.y < bot2   " << rect1.y << " + (" << rect1.h << " / 2) > " <<
			rect2.y << " + " << rect2.h;
		if (rect1.y + (rect1.h / 2) > rect2.y + rect2.h) {
			std::cout << " = True\n";
		} else {
			std::cout << " = False\n";
		}

		std::cout << "hit" << std::endl;
		*/
		return true;
	}
	return false;
}

bool Collisions::Pixel(Vector2D point1, Vector2D point2) {
	return (point1.x == point2.x) && (point1.y == point2.y);
}

bool Collisions::PointInArea(Point2D point, SDL_Rect area) {
	if (
		point.x > area.x &&
		point.x < area.x + area.w &&
		point.y > area.y &&
		point.y < area.y + area.h
	) { return true; }

	return false;
}
