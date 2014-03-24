#include "Game.h"
#include "YoungLady.h"
#include "Still.h"
#include "Room.h"
#include "Projectile.h"


YoungLady::YoungLady(Point2d c) :
	Player(c,1.0,1.5,0.520,10), shootCoolDown(0), facingRight(true), walkingFrames(0), spriteIndex(4)
{
	texture.loadFromFile("Lady.png");
	sprite.setOrigin(30,77);//pixel coordinates of the center of gravity of the young lady
	sprite.setPosition(sf::Vector2f(c.x,c.y));
	sprite.setTexture(texture);
}

YoungLady::~YoungLady(void)
{
	//empty
}

void YoungLady::simulate(){
	if(shootCoolDown>25){//if a projectile has just been shot
		spriteIndex=5;
	}else if(onGround){//if the lady is on the ground
		if(std::abs(speed.x)>0.1){//if she is walking
			spriteIndex=std::abs(2-walkingFrames/8);
			walkingFrames=(walkingFrames==31)? 0 : walkingFrames+1;
		}else{//if she is static
			spriteIndex=4;
		}
	}else{//if she is not on the ground
		spriteIndex=3;
	}
	sprite.setTextureRect(sf::IntRect(spriteIndex*88, 0, 88, 154));
	if(!facingRight)
		sprite.setScale(sf::Vector2f(-0.0097,0.0097));
	else
		sprite.setScale(sf::Vector2f(0.0097,0.0097));
	if(invincible>0)
		sprite.setColor(sf::Color(255,0,0));
	else if(invincible==0)
		sprite.setColor(sf::Color(255,255,255));
	Player::simulate();
	if(shootCoolDown>0)
		shootCoolDown--;
}

void YoungLady::displace(Point2d v){
	Character::displace(v);
	sprite.move(v.x,v.y);
}

void YoungLady::keyboard(Room* room){
	if(sf::Keyboard::isKeyPressed(leftKey)){
		force+=(onGround)? Point2d(-50,0) : Point2d(-10,0);
		facingRight=true;
	}
	if(sf::Keyboard::isKeyPressed(rightKey)){
		force+=(onGround)? Point2d(50,0) : Point2d(10,0);
		facingRight=false;
	}
	if(sf::Keyboard::isKeyPressed(jumpKey)&&onGround){
		force+=Point2d(0,-25000);
	}
}

void YoungLady::mouse(Room* room, sf::RenderWindow *window){
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)&&(shootCoolDown==0)){
		shootCoolDown=45;
		sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		Point2d dir (mousePosition.x-gravityCenter.x,mousePosition.y-gravityCenter.y);//direction of the projectile
		dir.normalized();
		std::vector<std::pair<int,int> > dmg;
		dmg.push_back(std::make_pair(1,RAW));
		room->projectiles.push_back(new BasicProjectile(gravityCenter+dir,0.2,0.2,dir*10.0,0,dmg,300.0));//shoot !
	}

}

void YoungLady::draw(sf::RenderWindow *window)const{
	if(sprite.getTexture()!=NULL){
		window->draw(sprite);
	}else{
		Character::draw(window);
	}
}