#pragma once

#include "SquareHitbox.h"

/*
a class to represent buttons
*/
class Button {
public :
	Button();
	Button(Point2d bottomLeft, Point2d topRight);
	virtual ~Button();

	/*
		should be called each frame to update mouseOverButton
	*/
	virtual bool isLeftClickedOn(sf::RenderWindow *window);
	virtual void draw(sf::RenderWindow *window) const =0;

protected :
	SquareHitbox hitbox;
};