#include "Spearman.h"
#include "SquareHitbox.h"
#include "Still.h"
#include "Game.h"
#include "Player.h"

Spearman::Spearman(Point2d c, bool right, Room* r) :
	Character(c,4.0,1.5,0.44,3), idle(true), facingRight(right), facingUp(false), turningProb(-180), walkingFrames(0), onGround(NULL), spriteIndex(0), room(r)
{
	Point2d shieldC (c.x+0.48,c.y-0.04);//center of gravity of the shield
	Hitbox* shieldBox = new SquareHitbox(Point2d(shieldC.x-0.5*0.11,shieldC.y-0.5*1.07),Point2d(shieldC.x+0.5*0.11,shieldC.y+0.5*1.07));
	shieldBox->setAttackID(1);
	hitboxes.push_back(hitboxes.front());//put the hitbox of the body at the end of the vector
	hitboxes.front()=shieldBox;//because the shield is a hitbox and an attackbox
	attackboxes.push_back(shieldBox);
	Point2d spearC (shieldC.x+0.36,shieldC.y);//center of gravity of the spear
	Hitbox* spearBox = new SquareHitbox(Point2d(spearC.x-0.5*0.39,spearC.y-0.5*0.19),Point2d(spearC.x+0.5*0.39,spearC.y+0.5*0.19));
	spearBox->setAttackID(0);
	attackboxes.push_back(spearBox);
	texture.loadFromFile("resources/images/spearman.png");
	sprite.setOrigin(sf::Vector2f(66,247));//pixel coordinates of the center of gravity of the spearman
	sprite.setPosition(c.x,c.y);
	sprite.setTexture(texture);
}

Spearman::~Spearman()
{
	//empty
}

void Spearman::resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){
	if(attackID==0){//spear
		fiend->damage(2,RAW,hitboxIndex);
		fiend->addToForce(d.normalize()*300.0);
	}else if(attackID==1){//shield
		fiend->addToForce(d.normalize()*400.0);
	}
}

void Spearman::damage(int deltaHP, int type, int hitboxIndex){
	if((invincible<1)&&(hitboxIndex==1)){
		damageNoTrigger(deltaHP,type);
		invincible= -1;
	}
}
void Spearman::damageNoTrigger(int deltaHP, int type, int hitboxIndex){
	if(HP-deltaHP<0){
		HP=0;
	}else{
		HP-=deltaHP;
	}
	idle=false;
}

void Spearman::resolveCollision(Still* other){
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

void Spearman::simulate(){
	Point2d v = room->player->getGravCenter()-gravityCenter;
	if(idle){//if the player has not been noticed yet
		Point2d facingDir;
		facingDir.x=(facingRight)? 1.0 : -1.0;
		idle = (v.normalize().dot(facingDir)<std::cos(PI/3.0))||(v.length2()>25.0);//the player is not noticed if far enough or above the spearman
		if(std::rand()%100<turningProb){//turn around
			facingRight=!facingRight;
			turningProb=-180;
		}else{//don't turn around
			turningProb++;
		}
	}else{
		facingRight=v.x>0;
		if(onGround!=NULL){//if the spearman is currently on the ground
			facingUp=false;
			if(-v.normalize().y>std::cos(PI/9.0)){
				force.x=0.0;
				force.y-=26000.0;
			}else{
				bool safe=false;
				unsigned int i=0;
				while((!safe)&&(i<onGround->getHitboxCount())){//is the center of gravity of the spearman above the ground or not ?
					safe=onGround->intersectRay(gravityCenter,Point2d(0.0,1.0));
					i++;
				}
				if(safe){//if it is, we are safe to run
					force.x+=(facingRight)? 20.0 : -20.0;
				}else{//if it is not, we should jump to not fall
					force.y-=5000.0;
				}
			}
		}else{//if the spearman is not on the ground
			force.x+=(facingRight)? 15.0 : -15.0;
			facingUp=(-v.normalize().y>std::cos(PI/9.0));
		}
	}
	Character::simulate();
	//moves attackboxes of the spear and the shield depending on the direction the spearman is facing
	if(facingRight){
		attackboxes.front()->setPosition(gravityCenter+Point2d(0.48,-0.04));
		attackboxes.back()->setPosition(gravityCenter+Point2d(0.84,-0.04));
	}else{
		attackboxes.front()->setPosition(gravityCenter+Point2d(-0.48,-0.04));
		attackboxes.back()->setPosition(gravityCenter+Point2d(-0.84,-0.04));
	}
	//select the right sprite to show
	if(facingUp){
		spriteIndex=4;
	}else if(onGround==NULL){
		spriteIndex=5;
	}else if(speed.x!=0.0){
		spriteIndex=walkingFrames/15;
		if(walkingFrames==59)
			walkingFrames=0;
		else
			walkingFrames++;
	}else{
		spriteIndex=0;
	}
	//sets the sprite right
	sprite.setTextureRect(sf::IntRect(spriteIndex*215, 0, 215, 380));
	if(!facingRight)
		sprite.setScale(sf::Vector2f(-0.007,0.007));
	else
		sprite.setScale(sf::Vector2f(0.007,0.007));
	if(invincible>0)
		sprite.setColor(sf::Color(255,0,0));
	else if(invincible==0)
		sprite.setColor(sf::Color(255,255,255));
	onGround=NULL;//reinitialize onGround
}

void Spearman::displace(Point2d v){
	Character::displace(v);
	sprite.move(v.x,v.y);
}


void Spearman::draw(sf::RenderWindow* window)const{
	if(sprite.getTexture()!=NULL){
		window->draw(sprite);
	}else{
		Character::draw(window);
	}
}

bool Spearman::loadFromFile(FILE* f, Room* r){
	if (f == NULL)
		return false;

	Point2d position;
	
	//have a temporary buffer used to read the file line by line...
	char buffer[1000];
	//this is where it happens.
	while (!feof(f)){
		//get a line from the file...
		Game::readValidLine(buffer, f, sizeof(buffer));
		char *line = Game::lTrim(buffer);

		int lineType = Game::getTypeID(line);
		switch (lineType) {
			case POSITION:
				if (sscanf(line, "%lf %lf", &position.x, &position.y) != 2)
					return false;
				break;
			case FACING_RIGHT:
				if (sscanf(line, "%d", &facingRight) != 1)
					return false;
				break;
			case END:
				room=r;
				displace(position);
				return true;
			default:
				return false;
		}
	}
	return false;
}