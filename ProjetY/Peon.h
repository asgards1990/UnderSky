#pragma once

#include "Character.h"

class Room;

class Peon : public Character{
public :
	Peon(Point2d c = Point2d(), bool right = true, Room* r = NULL);
	virtual ~Peon(void);

	void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	void damage(int deltaHP, int type, int hitboxIndex = 0);
	void damageNoTrigger(int deltaHP, int type, int hitboxIndex = 0);
	void resolveCollision(Still* other);
	void simulate();
	void displace(Point2d v);
	void draw(sf::RenderWindow* window)const;

	bool loadFromFile(FILE* f, Room* r);
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