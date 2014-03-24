#pragma once

#include <SFML\Graphics.hpp>
#include "Point2.h"

/*
mother class of all hitboxes
*/
class Hitbox
{
public:
	Hitbox(void) : attackID(-1){};
	virtual ~Hitbox(void){}

	virtual bool intersect(Hitbox* box, Point2d &d) const=0;//return true in case of intersection and write to "d" a displacement vector to set the hitboxes apart
	virtual bool isInside(Point2d p) const =0;//is point p inside this hitbox ?
	virtual bool intersectRay(Point2d o,Point2d d) const =0;//does the ray starting at o with direction d intersect this hitbox ?
	virtual void displace(Point2d v) =0; //displace the hitbox by applying vector "v"
	virtual void setPosition(Point2d x)=0;
	virtual void draw(sf::RenderWindow* window,sf::Color color) const=0;

	void setAttackID(int id){attackID=id;}
	int getAttackID()const{return attackID;}
	
protected :
	int attackID;//identifier of the attack inflicted by the hitbox woth respect to its owner. -1 if no attack is associated with it

};
