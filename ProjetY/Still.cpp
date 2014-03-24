#include <Windows.h>
#include <GL/GL.h>
#include "Still.h"
#include "SquareHitbox.h"

Still::~Still(void){

}

Still::Still(std::vector<Hitbox*> &p_hitboxes, char* path, int tileHeight, int tileWidth, std::vector<int> &tileIndices, std::vector<int> &tileRotations, int width, int height, Point2d origin, double scale) :
	Object(p_hitboxes), texturePath(path)
{
	if(texturePath!=NULL){
		if(!sprite.load(path,tileHeight,tileWidth,tileIndices,tileRotations,width,height)){
			std::cout << "Error : texture file " <<path<<" not found\n";
		}
		sprite.setPosition(origin.x,origin.y);
		sprite.setScale(scale,scale);
	}		
}

Still::Still(Point2d bottomLeft, Point2d topRight, char* path, int tileHeight, int tileWidth, std::vector<int> &tileIndices, std::vector<int> &tileRotations, int width, int height) :
	texturePath(path)
{
	hitboxes.push_back(new SquareHitbox(bottomLeft,topRight));
	if(texturePath!=NULL){
		if(!sprite.load(path,tileHeight,tileWidth,tileIndices,tileRotations,width,height)){
			std::cout << "Error : texture file " <<path<<" not found\n";
		}
		sprite.setPosition(bottomLeft.x,bottomLeft.y);
		sprite.setScale((topRight.x-bottomLeft.x)/(width*tileWidth),(topRight.y-bottomLeft.y)/(height*tileHeight));
	}	
}

void Still::draw(sf::RenderWindow* window)const{
	if(texturePath==NULL){
		for(unsigned int i=0;i<hitboxes.size();i++){
			hitboxes[i]->draw(window,sf::Color(255,255,255));
		}
	}else{
		window->draw(sprite);
	}
}