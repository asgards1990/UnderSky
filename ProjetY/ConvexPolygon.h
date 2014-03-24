#pragma once

#include <vector>
#include "Hitbox.h"

/*
a hitbox defined by a convex polygon
*/
class ConvexPolygon : public Hitbox{
public:
	ConvexPolygon(void){}
	ConvexPolygon(std::vector<Point2d> &v);
	~ConvexPolygon(void);

	bool intersect(Hitbox* other, Point2d &d) const;
	bool isInside(Point2d p) const;
	bool intersectRay(Point2d o, Point2d d) const;
	void setPosition(Point2d x);
	void displace(Point2d v);
	void draw(sf::RenderWindow* window,sf::Color color) const;

	std::vector<Point2d> vertices;
};

