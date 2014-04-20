#include "Room.h"
#include "Game.h"
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
	passiveExits.clear();
	activeExits.clear();
	fiends.clear();
}

bool Room::loadExitFromFile(FILE* f, Exit &exit){
	if (f == NULL)
		return false;

	//have a temporary buffer used to read the file line by line...
	char buffer[1000];
	//this is where it happens.
	while (!feof(f)){
		//get a line from the file...
		Game::readValidLine(buffer, f, sizeof(buffer));
		char *line = Game::lTrim(buffer);

		int lineType = Game::getTypeID(line);
		switch (lineType) {
			case SQUARE_HITBOX:{
					if (sscanf(line, "%lf %lf %lf %lf", &(exit.door.min.x), &(exit.door.min.y), &(exit.door.max.x), &(exit.door.max.y)) != 4)
						return false;
				}break;
			case ROOM_PATH:{
				strcpy(exit.nextRoom,Game::trim(line));
				}break;
			case POSITION:{
				if(sscanf(line,"%lf %lf",&exit.positionInNewRoom.x,&exit.positionInNewRoom.y)!=2)
						return false;
				}break;
			case HORIZONTAL:{
				if(sscanf(line,"%d",&exit.horizontal)!=1)
						return false;
				}break;
			case END:
				return true;
			default:
				return false;
		}
	}
	return false;
}