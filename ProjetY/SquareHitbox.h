#pragma once

#include "Hitbox.h"


/*
a rectangular box used to detect collisions
*/
class SquareHitbox : public Hitbox{
public:
	SquareHitbox();
	SquareHitbox(Point2d pt1, Point2d pt2);
	~SquareHitbox(void){};

	bool intersect(Hitbox* box, Point2d &d) const;
	bool isInside(Point2d p) const;
	bool intersectRay(Point2d o,Point2d d) const;
	void displace(Point2d v);
	void setPosition(Point2d x);
	void draw(sf::RenderWindow* window,sf::Color color) const;

	void include(Point2d p);
	Point2d min, max;
};

