#pragma once

#include <SFML\Window.hpp>
#include "Player.h"

class YoungLady : public Player
{
public:
	/*
	c : center of gravity
	*/
	YoungLady(Point2d c);
	~YoungLady(void);

	void simulate();
	void displace(Point2d v);

	void keyboard(Room* room);
	void mouse(Room* room, sf::RenderWindow *window);

	void draw(sf::RenderWindow *window)const;

protected :
	int shootCoolDown;//number of frames left till the projectile can be shot again

	bool facingRight;
	int walkingFrames;//a frame counter to display the walking animation properly

	sf::Sprite sprite;
	sf::Texture texture;
	int spriteIndex;//index of the image in the texture to display for the current frame 
};