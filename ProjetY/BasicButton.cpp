#include "BasicButton.h"

BasicButton::BasicButton(char* name, sf::Font &font, sf::Color textColor, sf::Color buttonColor, unsigned int size, Point2d position): 
	backgroundColor(buttonColor), buttonText (name,font,size)
{
	buttonText.setPosition(position.x,position.y);
	buttonText.setColor(textColor);
	double w = buttonText.getGlobalBounds().width;
	double h = buttonText.getGlobalBounds().height;
	Point2d textCenter = Point2d(buttonText.getGlobalBounds().left+0.5*w,buttonText.getGlobalBounds().top+0.5*h);
	hitbox.min = textCenter-Point2d(w*1.05+h*0.05,h*1.05+w*0.05);
	hitbox.max = textCenter+Point2d(w*1.05+h*0.05,h*1.05+w*0.05);
}

BasicButton::~BasicButton()
{
	//empty
}

bool BasicButton::isLeftClickedOn(sf::RenderWindow *window){
	sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	Point2d cursor (mousePosition.x,mousePosition.y);
	isMouseOverWindow =  hitbox.isInside(cursor);
	return sf::Mouse::isButtonPressed(sf::Mouse::Left)&&isMouseOverWindow;
}

void BasicButton::draw(sf::RenderWindow* window) const{
	if(isMouseOverWindow)
		hitbox.draw(window,backgroundColor);
	window->draw(buttonText);

}