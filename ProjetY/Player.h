#pragma once

#include <SFML\Window.hpp>
#include "Character.h"

class Room;

/*
a class to represent the main player
*/ 
class Player : public Character
{
public:
	Player(Point2d c, double m, double h, double w, int hp);
	virtual ~Player(void);

	virtual void simulate();
	virtual void resolveCollision(Character* other);
	virtual void resolveCollision(Still* other);

	/*
	gets and resolves keyboard inputs relative to the player
	*/
	virtual void keyboard(Room* room);

	/*
	gets and resolves mouse inputs relative to the player
	*/
	virtual void mouse(Room* room, sf::RenderWindow *window);

	void drawStats(sf::RenderWindow *window)const;

protected :
	sf::Keyboard::Key leftKey, rightKey, jumpKey;
	bool onGround;//is the player standing on the ground ?

	sf::Font arial;
};

