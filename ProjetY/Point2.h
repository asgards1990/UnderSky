#pragma once
#include <SFML/Graphics.hpp>

/*
a class to represent 2d vector or points with all useful operations
*/
template<typename T> class Point2
{
public:
	Point2(void){this->x = T(0); this->y = T(0);}
	Point2(double x, double y){this->x = x; this->y = y;}
	Point2(sf::Vector2<T> v){this->x=v.x; this->y=v.y;}
	~Point2(void){};

	void operator =(Point2 p){
		this->x=p.x;
		this->y=p.y;
	}

	Point2 operator +(Point2 p)const{
		return Point2(this->x+p.x,this->y+p.y);
	}

	Point2 operator -(Point2 p)const{
		return Point2(this->x-p.x,this->y-p.y);
	}

	Point2 operator *(T alpha)const{
		return Point2(alpha*this->x,alpha*this->y);
	}

	Point2 operator /(T alpha)const{
		return Point2(this->x/alpha,this->y/alpha);
	}

	void operator +=(Point2 p){
		this->x+=p.x;
		this->y+=p.y;
	}

	void operator -=(Point2 p){
		this->x-=p.x;
		this->y-=p.y;
	}

	void operator *=(T alpha){
		this->x*=alpha;
		this->y*=alpha;
	}

	void operator /=(T alpha){
		this->x/=alpha;
		this->y/=alpha;
	}

	T length()const{
		return sqrt(this->x*this->x+this->y*this->y);
	}

	T length2()const{
		return (this->x*this->x+this->y*this->y);
	}

	void normalized(){
		T l = length();
		this->x /= l;
		this->y /= l;
	}

	Point2 normalize()const{
		T l = length();
		return Point2(this->x/l,this->y/l);
	}

	T dot(Point2 other)const{
		return (this->x*other.x+this->y*other.y);
	}

	T cross(Point2 other)const{
		return this->x*other.y-this->y*other.x;
	}

	T x, y;
};


typedef Point2<double> Point2d;
