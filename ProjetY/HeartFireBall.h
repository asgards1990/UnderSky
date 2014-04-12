#pragma once

#include "Character.h"

class Player;

class HeartFireBall : public Character{
public :
	HeartFireBall(Point2d center, Point2d speed, Player* hero);

	void resolveCollision(Character* other);
	void resolveCollision(Still* other);
	void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	void simulate();

protected :
	Player* player;
};

