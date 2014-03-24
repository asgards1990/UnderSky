#pragma once

class Character;

class Status {
public :
	Status();
	virtual ~Status();

	virtual void init(Character* target){};
	virtual bool resolve(Character* target){};//returns true if the status should be deleted
};