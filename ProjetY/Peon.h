#pragma once

#include "Character.h"

class Room;

class Peon : public Character{
public :
	Peon(Point2d c, bool right, Room* r);
	virtual ~Peon(void);

	virtual void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	virtual void damage(int deltaHP, int type, int hitboxIndex = 0);
	virtual void damageNoTrigger(int deltaHP, int type, int hitboxIndex = 0);
	virtual void resolveCollision(Still* other);
	virtual void simulate();
	virtual void displace(Point2d v);
	virtual void draw(sf::RenderWindow* window)const;

protected :
	bool idle;
	bool facingRight;
	bool facingUp;
	int turningProb;
	int walkingFrames;
	Still* onGround;

	Room* room;

	sf::Sprite sprite;
	sf::Texture texture;
	int spriteIndex;
};