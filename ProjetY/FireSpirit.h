#pragma once

#include "HeartOfFire.h"

class Player;

class FireSpirit : public Character{
public :
	FireSpirit(Point2d c, bool right, int initialState, Player* r, HeartOfFire* boss);
	~FireSpirit(void);

	void resolveCollision(Character* other);
	void resolveCollision(Still* other);
	void damageNoTrigger(int deltaHP, int type, int hitboxIndex = 0);
	void simulate();
	void draw(sf::RenderWindow* window)const;

protected :
	bool facingRight;

	Player* player;
	HeartOfFire* hoF;

	int state;
	int stateChangeFrame;

	sf::Sprite sprite;
	int spriteIndex;//index of the image in the texture to display for the current frame 
};

enum{
	BURNING,
	DORMANT,
	RIOT
};