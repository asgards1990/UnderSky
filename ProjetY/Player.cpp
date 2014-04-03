#include <sstream>
#include "Game.h"
#include "Player.h"
#include "Still.h"
#include "Room.h"


Player::Player(Point2d c, double m, double height, double width, int hp) :
	Character(c,m,height,width,hp), onGround(false), leftKey(sf::Keyboard::Left), rightKey(sf::Keyboard::Right), jumpKey(sf::Keyboard::Up)
{
	arial.loadFromFile("resources/fonts/arial.ttf");
}

Player::~Player(void)
{
	//empty
}

void Player::simulate(){
	Character::simulate();
	onGround=false;
}

void Player::resolveCollision(Character* other){
	Character::resolveCollision(other);
}

void Player::resolveCollision(Still* other){
	for(unsigned int i=0;i<other->getHitboxCount();i++){
		for(unsigned int j=0;j<hitboxes.size();j++){
			Point2d d;
			if(other->getHitbox(i)->intersect(hitboxes[j],d)){
				double d2 = d.length2();
				if(d2!=0.0){
					displace(d);
					if(d.dot(speed)<0) speed = speed-d*d.dot(speed)/d2;
					if(d.y<0.0) onGround=true;//if the still is pushing the player upwards, we are standing on something
				}
			}
		}
	}
}

void Player::keyboard(Room* room){
	if(sf::Keyboard::isKeyPressed(leftKey)){
		force+=(onGround)? Point2d(-50,0) : Point2d(-10,0);
	}
	if(sf::Keyboard::isKeyPressed(rightKey)){
		force+=(onGround)? Point2d(50,0) : Point2d(10,0);
	}
	if(sf::Keyboard::isKeyPressed(jumpKey)&&onGround){
		force+=Point2d(0,-25000);
	}
}

void Player::mouse(Room* room, sf::RenderWindow *window){
	//empty
}

void Player::drawStats(sf::RenderWindow *window)const{
	sf::Text text;
	std::ostringstream ss;
	ss << HP << " / " << maxHP;
	text.setString(ss.str());
	text.setPosition(10.0f,10.0f);
	text.setFont(arial);
	text.setColor(sf::Color::White);
	text.setCharacterSize(40);
	window->draw(text);
}