#pragma once

#include "Button.h"

class BasicButton : public Button {
public :
	BasicButton(char* name, sf::Font &font, sf::Color textColor, sf::Color buttonColor, unsigned int size, Point2d position);
	~BasicButton();

	bool isLeftClickedOn(sf::RenderWindow *window);
	void draw(sf::RenderWindow *window) const;

protected :
	bool isMouseOverWindow;
	sf::Text buttonText;
	sf::Color backgroundColor;
};