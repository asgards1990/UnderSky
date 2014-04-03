#include "Button.h"

Button::Button():
	hitbox(Point2d(),Point2d())
{
	//empty
}

Button::Button(Point2d bottomLeft, Point2d topRight): 
	hitbox(bottomLeft,topRight)
{
	//empty
}

Button::~Button()
{
	//empty
}

bool Button::isLeftClickedOn(sf::RenderWindow *window){
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		Point2d cursor (mousePosition.x,mousePosition.y);
		return hitbox.isInside(cursor);
	}else{
		return false;
	}
}