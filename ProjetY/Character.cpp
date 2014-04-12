#include <windows.h>
#include "Game.h"
#include "Character.h"
#include "SquareHitbox.h"
#include "Still.h"
#include "Status.h"

Character::Character(void) : gravityCenter(Point2d()), mass(1.0),speed(Point2d()),force(Point2d()), maxHP(1), HP(1), invincible(0)
{

}

Character::Character(Point2d c, double m, double height, double width, int hp) :
	gravityCenter(c), mass(m), speed(Point2d()), force(Game::g*m), maxHP(hp), HP(hp), invincible(0)
{
	Hitbox* box = new SquareHitbox(Point2d(c.x-0.5*width,c.y-0.5*height),Point2d(c.x+0.5*width,c.y+0.5*height));
	hitboxes.push_back(box);
}


Character::~Character(void){
	for(unsigned int i=0;i<attackboxes.size();i++){
		//if a hitbox is also an attackbox, it must be at the beginning of both vectors not to be deleted twice
		if((i>hitboxes.size())||(attackboxes[i]!=hitboxes[i])){
			delete attackboxes[i];
		}
	}
	for(unsigned int i=0;i<statusInflicted.size();i++)
		delete statusInflicted[i];
}

void Character::resolveCollision(Character* other){
	for(unsigned int i=0;i<other->getAttackboxCount();i++){
		for(unsigned int j=0;j<hitboxes.size();j++){
			Point2d d;
			if(other->getAttackbox(i)->intersect(hitboxes[j],d)){//is there an intersection ?
				other->resolveAttack(other->getAttackbox(i)->getAttackID(),this,j,d);//if yes, resolve the attack !
			}
		}
	}
}

void Character::resolveCollision(Still* other){
	for(unsigned int i=0;i<other->getHitboxCount();i++){
		for(unsigned int j=0;j<hitboxes.size();j++){
			Point2d d;
			if(other->getHitbox(i)->intersect(hitboxes[j],d)){
				double d2 = d.length2();
				if(d2!=0){//is there an intersection ?
					displace(d);//displace to avoid collisions
					if(d.dot(speed)<0) speed = speed-d*d.dot(speed)/d2;//the collision absorbs the tangential velocity
				}
			}
		}
	}
}

void Character::resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){
}

void Character::simulate(){
	//using newton's equations
	speed=speed*(1-Game::friction*Game::getTimeStep()/mass)+force*Game::getTimeStep()/mass;
	//limit the velocity on each axis
	if(speed.x>Game::maxAxisSpeed) speed.x=Game::maxAxisSpeed;
	else if(speed.x<-Game::maxAxisSpeed) speed.x=-Game::maxAxisSpeed;
	if(speed.y>Game::maxAxisSpeed) speed.y=Game::maxAxisSpeed;
	else if(speed.y<-Game::maxAxisSpeed) speed.y=-Game::maxAxisSpeed;
	displace(speed*Game::getTimeStep());
	force = Game::g*mass;//reset forces

	for(int i=statusInflicted.size();i>0;i--)
		if(statusInflicted[i-1]->resolve(this))
			statusInflicted.erase(statusInflicted.begin()+i-1);

	if(invincible==-1){//character has been hit during this frame
		invincible=Game::invulnerability_frames;//character will not be able to get hit for some frames
	}else if(invincible>0){
		invincible--;
	}

}

void Character::displace(Point2d v){
	Object::displace(v);
	for(unsigned int i=0;i<attackboxes.size();i++){
		if((i>hitboxes.size())||(attackboxes[i]!=hitboxes[i])){
			attackboxes[i]->displace(v);
		}
	}
	gravityCenter+=v;
}

void Character::damage(int deltaHP, int type, int hitboxIndex){
	if(invincible<1){//if the current frame is not an invulnerability frame
		damageNoTrigger(deltaHP,type);
		invincible= -1;
	}
}

void Character::damageNoTrigger(int deltaHP, int type, int hitboxIndex){
	if(HP-deltaHP<0){
		HP=0;
	}else{
		HP-=deltaHP;
	}
}

void Character::heal(int deltaHP){
	if(HP+deltaHP>maxHP){
		HP=maxHP;
	}else{
		HP+=deltaHP;
	}
}

void Character::afflict(Status* status){
	status->init(this);
	statusInflicted.push_back(status);
}

void Character::cure(unsigned int i){
	delete statusInflicted[i];
	statusInflicted.erase(statusInflicted.begin()+i);
}

void Character::draw(sf::RenderWindow *window)const{
	for(unsigned int i=0;i<hitboxes.size();i++){//draw all hitboxes
		hitboxes[i]->draw(window,sf::Color(0,0,255));
	}
	for(unsigned int i=0;i<attackboxes.size();i++){//draw all attackboxes
		attackboxes[i]->draw(window,sf::Color(255,0,0));
	}
}