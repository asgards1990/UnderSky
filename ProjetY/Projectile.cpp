#include "Game.h"
#include "Projectile.h"
#include "Still.h"

BasicProjectile::BasicProjectile(Point2d center, double height, double width, Point2d vel, int frames, std::vector<std::pair<int,int> > dmg, double reject) :
	Character(center,1.0,height,width,1), damage(dmg), rejectionForce(reject), loading(frames)
{
	speed=vel;
	hitboxes.front()->setAttackID(0);
}

void BasicProjectile::resolveCollision(Still* other){
	for(unsigned int i=0;i<other->getHitboxCount();i++){
		for(unsigned int j=0;j<hitboxes.size();j++){
			Point2d d;
			if(other->getHitbox(i)->intersect(hitboxes[j],d)){
				HP=0;//destroy the projectile if it collides with a still
			}
		}
	}
}

void BasicProjectile::resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){
	if(attackID==0){
		for(unsigned int i=0;i<damage.size();i++){
			fiend->damage(damage[i].first,damage[i].second,hitboxIndex);//inflict damages
		}
		fiend->addToForce(d.normalize()*rejectionForce);//repel
		HP=0;//destroy the projetile
	}
}

void BasicProjectile::simulate(){
	displace(speed*Game::getTimeStep());
	if(loading>0){
		loading--;
	}else if(loading==0){//the projectile is now dangerous
		attackboxes.push_back(hitboxes.front());
		loading=-1;
	}
}