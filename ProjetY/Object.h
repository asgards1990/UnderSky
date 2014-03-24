#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>
#include "Point2.h"
#include "Hitbox.h"

class Character;
class Still;
class Camera;

/*
mother class of all objects in the game that can possibly collide with each other, being the player, a static object or an opponent
*/
class Object{
public:
	Object(void);
	Object(std::vector<Hitbox*> p_hitboxes);
	virtual ~Object(void);

	/*
	resolve all possible intersections between this object and "other"
	*/
	virtual void resolveCollision(Character* other){};
	virtual void resolveCollision(Still* other){};
	/*
	update position and velocity
	*/
	virtual void simulate(){};
	virtual void draw(sf::RenderWindow* window)const =0;
	/*
	displace the object by vector "v"
	*/
	virtual void displace(Point2d v);
	/*
	returns true if point p is inside one of the hitboxes of this object
	*/
	bool isInside(Point2d p) const;
	/*
	returns true if the ray emitted by point o with direction d hits one of the hitboxes of this object
	*/
	bool intersectRay(Point2d o, Point2d d) const;
	unsigned int getHitboxCount() const{return hitboxes.size();}
	const Hitbox* getHitbox(unsigned int i) const{return hitboxes[i];}

protected :
	std::vector<Hitbox*> hitboxes;//all hitboxes of the object used to determine collisions
};
