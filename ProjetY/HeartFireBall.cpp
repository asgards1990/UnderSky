#include "Game.h"
#include "HeartFireBall.h"
#include "Still.h"
#include "Player.h"
#include "Status.h"

HeartFireBall::HeartFireBall(Point2d center, Point2d vel, Player* hero) :
	Character(center,1.0,0.25,0.25,3), player(hero)
{
	speed=vel;
	hitboxes.front()->setAttackID(0);
	attackboxes.push_back(hitboxes.front());
}

void HeartFireBall::resolveCollision(Still* other){
	for(unsigned int i=0;i<other->getHitboxCount();i++){
		for(unsigned int j=0;j<hitboxes.size();j++){
			Point2d d;
			if(other->getHitbox(i)->intersect(hitboxes[j],d)){
				HP=0;//destroy the projectile if it collides with a still
			}
		}
	}
}

void HeartFireBall::resolveCollision(Character* other){
	Character::resolveCollision(other);
}

void HeartFireBall::resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){
	if(attackID==0){
		fiend->damage(3,FIRE,hitboxIndex);//inflict damages
		fiend->addToForce(d.normalize()*500.0);//repel
		HP=0;//destroy the projetile
	}
}

void HeartFireBall::simulate(){
	unsigned int i=0;
	while((i<player->getStatusCount())&&(player->getStatus(i)->getID()!=CURSE_OF_FIRE))
		i++;
	if(i!=player->getStatusCount()){//if the player is affected by the curse of fire, change the speed vector
		speed=(player->getGravCenter()-gravityCenter).normalize()*10.0;
	}
	displace(speed*Game::getTimeStep());
}