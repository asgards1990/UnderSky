#include <iostream>
#include "SquareHitbox.h"


SquareHitbox::SquareHitbox(void) :
	min(Point2d()), max(Point2d())
{
	//empty
}


SquareHitbox::SquareHitbox(Point2d pt1, Point2d pt2)
{
	if(pt1.x<pt2.x){
		min.x=pt1.x;
		max.x=pt2.x;
	}else{
		min.x=pt2.x;
		max.x=pt1.x;
	}
	if(pt1.y<pt2.y){
		min.y=pt1.y;
		max.y=pt2.y;
	}else{
		min.y=pt2.y;
		max.y=pt1.y;
	}
}

bool SquareHitbox::intersect(Hitbox* box, Point2d &d) const{
	SquareHitbox* square = dynamic_cast<SquareHitbox*>(box);
	d.x = 0;
	d.y = 0;
	double minLength = DBL_MAX;
	if(this->min.x>square->max.x)
		return false;
	else if(square->max.x-this->min.x<minLength){
		d = Point2d(this->min.x-square->max.x,0);
		minLength = square->max.x-this->min.x;
	}
	if(this->max.x<square->min.x)
		return false;
	else if(this->max.x-square->min.x<minLength){
		d = Point2d(this->max.x-square->min.x,0);
		minLength = this->max.x-square->min.x;
	}
	if(this->min.y>square->max.y)
		return false;
	else if(square->max.y-this->min.y<minLength){
		d = Point2d(0,this->min.y-square->max.y);
		minLength = square->max.y-this->min.y;
	}
	if(this->max.y<square->min.y)
		return false;
	else if(this->max.y-square->min.y<minLength){
		d = Point2d(0,this->max.y-square->min.y);
		minLength = this->max.y-square->min.y;
	}
	return true;
}

bool SquareHitbox::isInside(Point2d p) const{
	return (this->min.x<p.x)&&(this->max.x>p.x)&&(this->min.y<p.y)&&(this->max.y>p.y);
}

bool SquareHitbox::intersectRay(Point2d o, Point2d d) const{
	if(d.x==0.0)
		return (o.x>=min.x)&&(o.x<=max.x);
	else if(d.y==0.0)
		return (o.y>=min.y)&&(o.y<=max.y);
	else{
		double txmin, txmax, tymin, tymax, tmin, tmax;
		if(d.x>0){
			txmin = (min.x-o.x)/d.x;
			txmax = (max.x-o.x)/d.x;
		}else{
			txmin = (max.x-o.x)/d.x;
			txmax = (min.x-o.x)/d.x;
		}
		if(d.y>0){
			tymin = (min.y-o.y)/d.y;
			tymax = (max.y-o.y)/d.y;
		}else{
			tymin = (max.y-o.y)/d.y;
			tymax = (min.y-o.y)/d.y;
		}
		tmin = std::max(txmin,tymin);
		tmax = std::min(txmax,tymax);
		return (tmin<=tmax)&&(tmax>=0);
	}
}

void SquareHitbox::include(Point2d p){
	if(this->min.x>p.x){
		this->min.x=p.x;
	}else if(this->max.x<p.x){
		this->max.x=p.x;
	}
	if(this->min.y>p.y){
		this->min.y=p.y;
	}else if(this->max.y<p.y){
		this->max.y=p.y;
	}
}

void SquareHitbox::displace(Point2d v){
	min+=v;
	max+=v;
}

void SquareHitbox::setPosition(Point2d x){
	displace(x-(min+max)*0.5);
}

void SquareHitbox::draw(sf::RenderWindow* window,sf::Color color) const{
	sf::RectangleShape rect (sf::Vector2f(max.x-min.x,max.y-min.y));
	rect.setFillColor(color);
	rect.setPosition(min.x,min.y);
	window->draw(rect);
}