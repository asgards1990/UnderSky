#pragma once

#include "Character.h"

class Room;

class Spearman : public Character{
public :
	Spearman(Point2d c = Point2d(), bool right = true, Room* r = NULL);
	virtual ~Spearman(void);

	virtual void resolveCollision(Still* other);
	virtual void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	virtual void damage(int deltaHP, int type, int hitboxIndex = 0);
	virtual void damageNoTrigger(int deltaHP, int type, int hitboxIndex = 0);
	virtual void simulate();
	virtual void displace(Point2d v);
	virtual void draw(sf::RenderWindow* window)const;

	bool loadFromFile(FILE* f, Room* r);
protected :
	bool idle;//true if the player has not been noticed yet
	bool facingRight;
	bool facingUp;
	int turningProb;//the greater the integer, the likelier it is for the spearman to turn around while the player has not been noticed
	int walkingFrames;//a frame counter to display the walking animation properly
	Still* onGround;//a pointer to the still the spearman is standing on

	Room* room;//the room the spearman is in

	sf::Sprite sprite;
	sf::Texture texture;
	int spriteIndex;//index of the image in the texture to display for the current frame 
};