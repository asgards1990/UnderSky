#include "Room.h"
#include "Player.h"
#include "Still.h"

Room::Room() : player(NULL)
{

}

Room::~Room(){
	empty();
	delete player;
}

void Room::empty(){
	unsigned int i;
	for(i=0;i<scenery.size();i++){
		delete scenery[i];
	}
	scenery.clear();
	for(i=0;i<fiends.size();i++){
		delete fiends[i];
	}
	fiends.clear();
	for(i=0;i<projectiles.size();i++){
		delete projectiles[i];
	}
	fiends.clear();
	for(i=0;i<passiveExits.size();i++){
		delete passiveExits[i].first;
	}
	passiveExits.clear();
	for(i=0;i<activeExits.size();i++){
		delete activeExits[i].first;
	}
	activeExits.clear();
}