#include "ConvexPolygon.h"
#include "SquareHitbox.h"


ConvexPolygon::ConvexPolygon(std::vector<Point2d> &v) :
	vertices(v)
{
	//empty
}


ConvexPolygon::~ConvexPolygon(void)
{
}

void ConvexPolygon::setPosition(Point2d x){
	Point2d barycenter;
	for(unsigned int i=0; i<vertices.size();i++) barycenter+=vertices[i];
	barycenter/=vertices.size();
	displace(x-barycenter);
}

void ConvexPolygon::displace(Point2d v){
	for(unsigned int i=0; i<vertices.size();i++) vertices[i]+=v;
}

/*
the intersection test between two convex polygon is done using the separating axis thoerem :
that is, two convex polygons don't intersect if you can draw a straight line between them.
So we project both polygons on all lines orthogonal to an edge of one of the two polygons,
and we check if the projected polygons (now in 1D) intersect.
The direction and magnitude of the repel vector "d" if given using the projection with the
smallest overlapping.
*/
bool ConvexPolygon::intersect(Hitbox* other, Point2d &d) const{
	SquareHitbox* square = dynamic_cast<SquareHitbox*>(other);
	double dLength2 = DBL_MAX;
	std::vector<Point2d> otherVertices;
	//a rectangular hitbox is also a convex polygon, so we treat it the same way
	if(square){
		otherVertices.resize(4);
		otherVertices[0]=square->min;
		otherVertices[1]=Point2d(square->min.x,square->max.y);
		otherVertices[2]=square->max;
		otherVertices[3]=Point2d(square->max.x,square->min.y);
	}else{
		ConvexPolygon* polygon = dynamic_cast<ConvexPolygon*>(other);
		otherVertices = polygon->vertices;

	}
	//we project on line orthogonal to edges of the first polygon
	for(unsigned int i=0;i<vertices.size();i++){
		Point2d normal (vertices[i].y-vertices[(i+1)%vertices.size()].y,vertices[(i+1)%vertices.size()].x-vertices[i].x);
		normal.normalize();
		double otherMin = normal.dot(otherVertices[0]);
		double otherMax = otherMin;
		//we project the second polygon
		for(unsigned int j=1;j<otherVertices.size();j++){
			double projection = normal.dot(otherVertices[j]);
			if(projection<otherMin)
				otherMin = projection;
			else if(projection>otherMax)
				otherMax = projection;
		}
		double myMin = normal.dot(vertices[0]);
		double myMax = myMin;
		//we project the first polygon
		for(unsigned int j=1;j<vertices.size();j++){
			double projection = normal.dot(vertices[j]);
			if(projection<myMin)
				myMin = projection;
			else if(projection>myMax)
				myMax = projection;
		}
		//we test whether the projected polygons overlap or not
		if((myMin>otherMax)||(myMax<otherMin)){
			return false;
		}else{
			if(otherMax-myMin>myMax-otherMin){
				double l2=(myMax-otherMin)*(myMax-otherMin);
				if(l2<dLength2){
					d = normal*(myMax-otherMin);
					dLength2=l2;
				}
			}else{
				double l2=(otherMax-myMin)*(otherMax-myMin);
				if(l2<dLength2){
					d = normal*(myMin-otherMax);
					dLength2=l2;
				}
			}
		}
	}
	//we project on line orthogonal to edges of the second polygon
	for(unsigned int i=0;i<otherVertices.size();i++){
		Point2d normal (otherVertices[i].y-otherVertices[(i+1)%otherVertices.size()].y,otherVertices[(i+1)%otherVertices.size()].x-otherVertices[i].x);
		normal.normalize();
		double otherMin = normal.dot(otherVertices[0]);
		double otherMax = otherMin;
		//we project the second polygon
		for(unsigned int j=1;j<otherVertices.size();j++){
			double projection = normal.dot(otherVertices.at(j));
			if(projection<otherMin)
				otherMin = projection;
			else if(projection>otherMax)
				otherMax = projection;
		}
		double myMin = normal.dot(vertices[0]);
		double myMax = myMin;
		//we project the first polygon
		for(unsigned int j=1;j<vertices.size();j++){
			double projection = normal.dot(vertices[j]);
			if(projection<myMin)
				myMin = projection;
			else if(projection>myMax)
				myMax = projection;
		}
		//we test whether the projected polygons overlap or not
		if((myMin>otherMax)||(myMax<otherMin)){
			return false;
		}else{
			if(otherMax-myMin>myMax-otherMin){
				double l2 = (myMax-otherMin)*(myMax-otherMin);
				if(l2<dLength2){
					d = normal*(myMax-otherMin);
					dLength2=l2;
				}
			}else{
				double l2 = (otherMax-myMin)*(otherMax-myMin);
				if(l2<dLength2){
					d = normal*(myMin-otherMax);
					dLength2=l2;
				}
			}
		}
	}
	return true;
}

bool ConvexPolygon::intersectRay(Point2d o, Point2d d) const{
	for(unsigned int i=0;i<vertices.size();i++){
		Point2d ac=(i==0)? o-vertices.back() : o-vertices[i-1];
		Point2d cd=d;
		Point2d ab=(i==0)? vertices.front()-vertices.back() : vertices[i]-vertices[i-1];
		double denom=ab.x*cd.y-cd.x-ab.y;
		if(denom!=0.0){
			double t = (ac.x*cd.y+cd.x*ac.y)/denom;
			if((t>=0.0)&&(t<=1.0)&&(cd.dot(ab*t-ac)>=0.0)){
				return true;
			}
		}
	}
	return false;
}

bool ConvexPolygon::isInside(Point2d p) const{
	bool sign = (vertices.front()-vertices.back()).cross(p-vertices.back())>0;
	unsigned int i=1;
	while((i<vertices.size())&&(sign==((vertices[i]-vertices[i-1]).cross(p-vertices[i-1])>0))) i++;
	return i==vertices.size();
}

void ConvexPolygon::draw(sf::RenderWindow* window,sf::Color color) const{
	sf::ConvexShape convex;
	convex.setPointCount(vertices.size());
	convex.setFillColor(color);
	for(unsigned int i=0;i<vertices.size();i++){
		convex.setPoint(i,sf::Vector2f(vertices[i].x,vertices[i].y));
	}
	window->draw(convex);
}