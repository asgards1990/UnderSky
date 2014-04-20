#pragma once

#include <vector>
#include "SquareHitbox.h"

class Player;
class Still;
class Character;

struct Exit{
	SquareHitbox door;
	char nextRoom [200];
	Point2d positionInNewRoom;
	bool horizontal;
};

class Room{
public :
	Room();
	~Room();

	void empty();

	std::vector<Still*> scenery;
	std::vector<Character*> fiends;
	std::vector<Character*> projectiles;
	Player* player;
	SquareHitbox boundingBox;
	std::vector<Exit> passiveExits;//hitbox of exits and path to the file describing the associated room
	std::vector<Exit> activeExits;//exits requiring to push a button

	bool loadExitFromFile(FILE* f, Exit &e);
};