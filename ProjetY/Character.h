#pragma once

#include "Object.h"

class Still;
class Status;

/*
a class to represent moving objects, be it the player or an opponent or a projectile
*/
class Character : public Object{
public:
	Character(void);
	/*
	c : center of gravity in world coordinates
	m : mass
	h : height
	w : width
	hp : health points
	*/
	Character(Point2d c, double m, double h, double w, int hp);
	virtual ~Character(void);

	virtual void resolveCollision(Character* other);
	virtual void resolveCollision(Still* other);
	/*
	resolve attack of identifier "attackID" of this object on hitbox "hitboxIndex" of character "fiend" with separation vector "d"
	*/
	virtual void resolveAttack(int attackID, Character* fiend, int hitboxIndex, Point2d d); 
	/*
	inflict damage of amount "deltaHP" and type "type" on hitbox "hitboxIndex" of this object
	*/
	virtual void damage(int deltaHP, int type, int hitboxIndex = 0);
	/*
	damage this object without triggering invulnerability frames
	*/
	virtual void damageNoTrigger(int deltaHP, int type, int hitboxIndex = 0);
	virtual void heal(int deltaHP);
	virtual void simulate();
	virtual void displace(Point2d v);
	virtual void draw(sf::RenderWindow* window)const;

	Point2d getGravCenter(){return gravityCenter;}
	Point2d getV()const {return speed;}
	double getMass()const {return mass;}
	unsigned int getAttackboxCount()const{return attackboxes.size();}
	const Hitbox* getAttackbox(unsigned int i)const{return attackboxes[i];}
	void addToForce(Point2d f){force+=f;}
	/*
	afflict the character with a status
	*/
	void afflict (Status* status);
	/*
	remove a status
	*/
	void cure(unsigned int i);
	unsigned int getStatusCount()const{return statusInflicted.size();}
	const Status* getStatus(int index)const {return statusInflicted[index];}
	bool isDead(){return HP==0;}

protected:
	std::vector<Hitbox*> attackboxes;

	Point2d gravityCenter;//center of gravity of the character
	double mass;
	Point2d speed;//velocity
	Point2d force;//total forces applied on this object for the current time step

	int maxHP;
	int HP;
	int invincible;//==-1 of this object was damaged during this frame, equal to the number of remaining invulnerability frames in all other cases
	std::vector<Status*> statusInflicted;
};

