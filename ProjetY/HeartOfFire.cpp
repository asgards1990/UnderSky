#include "HeartOfFire.h"
#include "SquareHitbox.h"
#include "Still.h"
#include "Game.h"
#include "Player.h"
#include "HeartFireBall.h"

HeartOfFire::HeartOfFire(Point2d c, bool right, Room* r) :
	Character(c,5.0,1.5,0.5,20), idle(true), facingRight(right), walkingFrames(0), onGround(NULL), spriteIndex(0), room(r), heatWaveFrame(0), heatBar(0), heatSalvationCooldown(1800), fireBallCooldown(60), dodgeCooldown(0)
{
	heatWave.setPrimitiveType(sf::TrianglesFan);
	heatWave.resize(74);
	heatWave[0].color=sf::Color(255,0,0,100);
	heatWave[0].position=sf::Vector2f(c.x,c.y);
	for(int i=1;i<74;i++){
		heatWave[i].color=sf::Color(255,0,0,0);
		heatWave[i].position=sf::Vector2f(c.x+10.0*std::cos(PI*i/36.0),c.y+10.0*std::sin(PI*i/36.0));
	}
		
}

HeartOfFire::~HeartOfFire()
{
	//empty
}

void HeartOfFire::resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d){}

void HeartOfFire::damage(int deltaHP, int type, int hitboxIndex){
	if(invincible<1){
		damageNoTrigger(deltaHP,type);
		invincible= -1;
	}
}
void HeartOfFire::damageNoTrigger(int deltaHP, int type, int hitboxIndex){
	if(HP-deltaHP<0){
		HP=0;
	}else{
		HP-=deltaHP;
	}
	idle=false;
}

void HeartOfFire::resolveCollision(Still* other){
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

void HeartOfFire::simulate(){
	Point2d playerDirection = room->player->getGravCenter()-gravityCenter;
	double playerDistance2=playerDirection.length2();
	//heat wave radius change
	double heatRadius=10.0+2*sin(heatWaveFrame*PI/30.0);
	for(unsigned int i=1;i<heatWave.getVertexCount();i++){
		Point2d ray (heatWave[i].position.x-heatWave[0].position.x,heatWave[i].position.y-heatWave[0].position.y);
		ray.normalized();
		ray*=heatRadius;
		heatWave[i].position=sf::Vector2f(heatWave[0].position.x+ray.x,heatWave[0].position.y+ray.y);
	}
	if(heatWaveFrame==60)
		heatWaveFrame=1;
	else
		heatWaveFrame++;
	//heat wave effects
	if(playerDistance2>heatRadius*heatRadius){//if sufficiently far away, the heat bar empties itself
		if(heatBar>0)
			heatBar--;
	}else{//the closer the player is, the faster the bar increases
		heatBar+=5*(1.0-playerDistance2/(heatRadius*heatRadius));
		if(heatBar>900)//damages
			room->player->damageNoTrigger(1.0,FIRE,0);
	}
	facingRight=playerDirection.x>0.0;
	int closestProjectile=-1;
	double smallestLength2=16.0;
	for(unsigned int i=0;i<room->projectiles.size();i++){//what is the closest incoming projectile less than 4.0 away ?
		Point2d projectileDirection = room->projectiles[i]->getGravCenter()-gravityCenter;
		if((projectileDirection.length2()<smallestLength2)&&(hitboxes.front()->intersectRay(room->projectiles[i]->getGravCenter(),room->projectiles[i]->getV()))){
			closestProjectile=i;
			smallestLength2=(gravityCenter-room->projectiles[i]->getGravCenter()).length2();
		}
	}
	Point2d closestProjectileDirection;
	if(closestProjectile!=-1)
		closestProjectileDirection=room->projectiles[closestProjectile]->getGravCenter()-gravityCenter;
	if((heatSalvationCooldown==0)&&(dodgeCooldown==0)){//heat salvation summoning !
		//TODO : spirits pop
		heatSalvationCooldown=1800;
	}else if((fireBallCooldown==0)&&(dodgeCooldown==0)){//fireball !
		Point2d shotDirection;
		Point2d shotOrigin=(facingRight)? gravityCenter+Point2d(0.7,0.1) : gravityCenter-Point2d(0.7,0.1);
		//if a projectile is incoming, shoot at it ! 
		if((closestProjectile!=-1)&&((facingRight&&(closestProjectileDirection.x>0.0))||(!facingRight&&(closestProjectileDirection.x<0.0))))
			shotDirection = closestProjectileDirection;//direction of the projectile
		else
			shotDirection=playerDirection;
		shotDirection+=(facingRight)? Point2d(0.7,0.1) : Point2d(-0.7,0.1);		
		shotDirection.normalized();
		std::vector<std::pair<int,int> > dmg;
		dmg.push_back(std::make_pair(3,FIRE));
		room->projectiles.push_back(new HeartFireBall(shotOrigin,shotDirection*10,room->player));//shoot !
		fireBallCooldown=45+rand()%31;//random cooldown
	}else if(onGround){
		if(closestProjectile!=-1){
			if(closestProjectileDirection.normalize().y<-std::cos(PI/3.0)){//a projectile is coming from above, dodge right or left
				if(dodgeCooldown==0){
					if(closestProjectileDirection.x>0.0){
						force.x-=4000.0;
						facingRight=false;
					}else{
						force.x+=4000.0;
						facingRight=true;
					}
					dodgeCooldown=20;
				}
			}else{//a projectile is coming from right or left, so jump !
				force.y-=5000.0;
			}
		}else{
			force.x+=(facingRight)? 5.0 : -5.0;
		}
	}
	if(fireBallCooldown>0)
		fireBallCooldown--;
	if(heatSalvationCooldown>0)
		heatSalvationCooldown--;
	if(dodgeCooldown>0)
		dodgeCooldown--;
	Character::simulate();
	onGround=NULL;//reinitialize onGround
}

void HeartOfFire::displace(Point2d v){
	Character::displace(v);
	sprite.move(v.x,v.y);
	for(unsigned int i=0;i<heatWave.getVertexCount();i++)
		heatWave[i].position+=sf::Vector2f(v.x,v.y);
}


void HeartOfFire::draw(sf::RenderWindow* window)const{
	Character::draw(window);
	window->draw(heatWave);
}

bool HeartOfFire::loadFromFile(FILE* f, Room* r){
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