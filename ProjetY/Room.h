#pragma once

#include <vector>
#include "SquareHitbox.h"

class Player;
class Still;
class Character;

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
	std::vector<std::pair<SquareHitbox*,char*> > passiveExits;//hitbox of exits and path to the file describing the associated room
	std::vector<std::pair<SquareHitbox*,char*> > activeExits;//exits requiring to push a button
};