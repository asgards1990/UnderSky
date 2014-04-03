#pragma once

#include "Character.h"

class Room;

class HeartOfFire : public Character{
public :
	HeartOfFire(Point2d c, bool right, Room* r);
	virtual ~HeartOfFire(void);

	virtual void resolveCollision(Still* other);
	virtual void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	virtual void damage(int deltaHP, int type, int hitboxIndex = 0);
	virtual void damageNoTrigger(int deltaHP, int type, int hitboxIndex = 0);
	virtual void simulate();
	virtual void displace(Point2d v);
	virtual void draw(sf::RenderWindow* window)const;

protected :
	bool idle;//true if the player has not been noticed yet
	bool facingRight;
	int walkingFrames;//a frame counter to display the walking animation properly
	Still* onGround;//a pointer to the still the spearman is standing on

	Room* room;//the room the spearman is in

	int heatBar;
	int heatSalvationCooldown;
	int fireBallCooldown;
	int dodgeCooldown;
	int heatWaveFrame;

	sf::Sprite sprite;
	sf::VertexArray heatWave;
	sf::Texture texture;
	int spriteIndex;//index of the image in the texture to display for the current frame 
};