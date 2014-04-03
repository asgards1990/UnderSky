#pragma once

#include "Character.h"

class Player;

class HeartFireBall : public Character{
public :
	HeartFireBall(Point2d center, Point2d speed, Player* player){};

	virtual void resolveCollision(Character* other){};
	virtual void resolveCollision(Still* other){};
	virtual void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){}; 
	virtual void simulate(){};

protected :
	Player* player;
};

