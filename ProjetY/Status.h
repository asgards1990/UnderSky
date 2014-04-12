#pragma once

#include <SFML/Graphics.hpp>

class Character;

enum StatusID{
	CURSE_OF_FIRE
};

class Status {
public :
	Status();
	virtual ~Status();

	virtual void init(Character* target)=0;
	/*
	returns true if the status should be deleted
	*/
	virtual bool resolve(Character* target)=0;
	virtual sf::Color draw(sf::RenderWindow *window)=0;
	virtual StatusID getID()const =0;
};