#include "Peon.h"
#include "SquareHitbox.h"
#include "Still.h"
#include "Game.h"
#include "Player.h"

Peon::Peon(Point2d c, bool right, Room* r) :
	Character(c,4.0,1.5,0.44,3), idle(true), facingRight(right), facingUp(false), turningProb(-180), walkingFrames(0), onGround(NULL), spriteIndex(0), room(r)
{
	Point2d swordC (c.x+0.36,c.y-0.04);
	Hitbox* swordBox = new SquareHitbox(Point2d(swordC.x-0.5*0.39,swordC.y-0.5*0.19),Point2d(swordC.x+0.5*0.39,swordC.y+0.5*0.19));
	swordBox->setAttackID(0);
	attackboxes.push_back(swordBox);
	texture.loadFromFile("resources/images/Peon.png");
	sprite.setOrigin(sf::Vector2f(66,247));
	sprite.setPosition(c.x,c.y);
	sprite.setTexture(texture);
}

Peon::~Peon()
{
	//empty
}

void Peon::resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){
	if(attackID==0){
		fiend->damage(2,RAW,hitboxIndex);
		fiend->addToForce(d.normalize()*300.0);
	}
}

void Peon::damage(int deltaHP, int type, int hitboxIndex){
	if((invincible<1)&&(hitboxIndex==1)){
		damageNoTrigger(deltaHP,type);
		invincible= -1;
	}
}
void Peon::damageNoTrigger(int deltaHP, int type, int hitboxIndex){
	if(HP-deltaHP<0){
		HP=0;
	}else{
		HP-=deltaHP;
	}
	idle=false;
}

void Peon::resolveCollision(Still* other){
	for(unsigned int i=0;i<other->getHitboxCount();i++){
		for(unsigned int j=0;j<hitboxes.size();j++){
			Point2d d;
			if(other->getHitbox(i)->intersect(hitboxes[j],d)){
				double d2 = d.length2();
				if(d2!=0.0){
					displace(d);
					if(d.dot(speed)<0) speed = speed-d*d.dot(speed)/d2;
					if(d.y<0.0) onGround=other;//if the still is pushing the player upwards, we are standing on something
				}
			}
		}
	}
}

void Peon::simulate(){
	Point2d v = room->player->getGravCenter()-gravityCenter;
	if(idle){
		Point2d facingDir;
		facingDir.x=(facingRight)? 1.0 : -1.0;
		idle = (v.normalize().dot(facingDir)<std::cos(PI/3.0))||(v.length2()>25.0);
		if(std::rand()%100<turningProb){
			facingRight=!facingRight;
			turningProb=-180;
		}else{
			turningProb++;
		}
	}else{
		facingRight=v.x>0;
		if(onGround!=NULL){
			facingUp=false;
			if(-v.normalize().y>std::cos(PI/9.0)){
				force.x=0.0;
				force.y-=26000.0;
			}else{
				bool safe=false;
				unsigned int i=0;
				while((!safe)&&(i<onGround->getHitboxCount())){
					safe=onGround->intersectRay(gravityCenter,Point2d(0.0,1.0));
					i++;
				}
				if(safe){
					if(v.length2()>16.5) {
						force.x+=(facingRight)? 2.5 : -2.5;}
					else if(v.length2()>3.5){
						force.x+=(facingRight)? 32.0 : -32.0;
					}
					else {
						force.x+=(facingRight)? 7.0 : -7.0;
					}
				}else{
					force.y-=5000.0;
				}
			}
		}else{
			force.x+=(facingRight)? 15.0 : -15.0;
			facingUp=(-v.normalize().y>std::cos(PI/9.0));
		}
	}
	Character::simulate();
	if(facingRight){
		attackboxes.front()->setPosition(gravityCenter+Point2d(0.48,-0.04));
		attackboxes.back()->setPosition(gravityCenter+Point2d(0.84,-0.04));
	}else{
		attackboxes.front()->setPosition(gravityCenter+Point2d(-0.48,-0.04));
		attackboxes.back()->setPosition(gravityCenter+Point2d(-0.84,-0.04));
	}
	if(facingUp){
		spriteIndex=3;
	}else if(onGround==NULL){
		spriteIndex=4;
	}else if(speed.x!=0.0&&v.length2()>16.5){
		spriteIndex=1;
	}
	else {
		spriteIndex=0;
	}
	sprite.setTextureRect(sf::IntRect(spriteIndex*215, 0, 215, 350));
	if(!facingRight)
		sprite.setScale(sf::Vector2f(0.007,0.007));
	else
		sprite.setScale(sf::Vector2f(-0.007,0.007));
	if(invincible>0)
		sprite.setColor(sf::Color(255,0,0));
	else if(invincible==0)
		sprite.setColor(sf::Color(255,255,255));
	onGround=NULL;
}

void Peon::displace(Point2d v){
	Character::displace(v);
	sprite.move(v.x,v.y);
}


void Peon::draw(sf::RenderWindow* window)const{
	if(sprite.getTexture()!=NULL){
		window->draw(sprite);
	//}else{
		Character::draw(window);
	}
}
