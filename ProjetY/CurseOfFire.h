#pragma once

#include "Status.h"

class CurseOfFire : public Status {
public :
	CurseOfFire():countdown(0){};
	~CurseOfFire(){};

	void init(Character* target){countdown=481;}
	bool resolve(Character* target){if(countdown>0) countdown--; return countdown==0;}
	sf::Color draw(sf::RenderWindow *window){return sf::Color(200,0,0);}
	StatusID getID()const{return CURSE_OF_FIRE;}	
protected :
	int countdown;
};