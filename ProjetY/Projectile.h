#pragma once

#include "Character.h"

class BasicProjectile : public Character{
public :
	BasicProjectile(Point2d center, double height, double width, Point2d speed, int frames, std::vector<std::pair<int,int> > dmg, double reject);

	virtual void resolveCollision(Character* other){};
	virtual void resolveCollision(Still* other);
	virtual void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	virtual void simulate();

protected :
	int loading;//number of frames during which the projectile is harmless
	std::vector<std::pair<int,int> > damage;//damages inflicted upon contact
	double rejectionForce;//magnitude of the force applied in case of collision
};