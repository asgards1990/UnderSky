#include "FireSpirit.h"
#include "Player.h"
#include "Game.h"

FireSpirit::FireSpirit(Point2d c, bool right, int initialState, Player* hero, HeartOfFire* boss) :
	Character(c,1.0,0.5,0.5,1), state(initialState), player(hero), hoF(boss), stateChangeFrame(180)
{
	//empty
}

FireSpirit::~FireSpirit(){
}

void FireSpirit::resolveCollision(Character* other){
}

void FireSpirit::resolveCollision(Still* other){
}

void FireSpirit::damageNoTrigger(int deltaHP, int type, int hitboxIndex){
}

void FireSpirit::simulate(){
	Point2d bossDirection = hoF->getGravCenter()-gravityCenter;
	double bossDistance2 = bossDirection.length2();
	double playerDistance2 = (player->getGravCenter()-gravityCenter).length2();
	if(bossDistance2<0.1){
		if(state==DORMANT){
			hoF->heal(10);
		}else if(state==BURNING){
			//fire affinity
		}else if(state==RIOT){
			//attack
		}
		HP=0;
	}else if(playerDistance2<0.1){
		if(state==DORMANT){
			player->heal(2);
		}else if(state==BURNING){
			player->damageNoTrigger(2,FIRE,0);
		}else if(state==RIOT){
			//curse of Fire
		}
		HP=0;
	}
	speed=bossDirection.normalize()*5.0;
	if(stateChangeFrame>0){
		stateChangeFrame--;
	}else{
		state=(state+1)%3;
		stateChangeFrame=180;
	}
}

void FireSpirit::draw(sf::RenderWindow* window)const{
	Character::draw(window);
}