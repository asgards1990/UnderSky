#include "Object.h"
#include "Hitbox.h"

Object::Object()
{
	//empty
}

Object::~Object(void){
	for(unsigned int i=0;i<hitboxes.size();i++){
		delete hitboxes[i];
	}
}

Object::Object(std::vector<Hitbox*> p_hitboxes) :
	hitboxes(p_hitboxes)
{
	//empty
}

void Object::displace(Point2d v){
	for(unsigned int i=0;i<hitboxes.size();i++){
		hitboxes[i]->displace(v);
	}
}

bool Object::isInside(Point2d p) const{
	unsigned int i=0;
	while((i<hitboxes.size())&&(!hitboxes[i]->isInside(p))) i++;
	return i!=hitboxes.size();
}

bool Object::intersectRay(Point2d o, Point2d d) const{
	unsigned int i=0;
	while((i<hitboxes.size())&&(!hitboxes[i]->intersectRay(o,d))) i++;
	return i!=hitboxes.size();
}