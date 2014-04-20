#include <Windows.h>
#include <GL/GL.h>
#include "Still.h"
#include "SquareHitbox.h"
#include "ConvexPolygon.h"
#include "Game.h"

Still::~Still(void){

}

Still::Still(std::vector<Hitbox*> &p_hitboxes, char* path, int tileWidth, int tileHeight, std::vector<int> &tileIndices, std::vector<int> &tileRotations, int width, int height, Point2d origin, double scale) :
	Object(p_hitboxes)
{
	if(path!=NULL){
		if(!sprite.load(path,tileWidth,tileHeight,tileIndices,tileRotations,width,height)){
			std::cout << "Error : texture file " <<path<<" not found\n";
		}
		sprite.setPosition(origin.x,origin.y);
		sprite.setScale(scale,scale);
	}		
}

Still::Still(Point2d bottomLeft, Point2d topRight, char* path, int tileWidth, int tileHeight, std::vector<int> &tileIndices, std::vector<int> &tileRotations, int width, int height)
{
	hitboxes.push_back(new SquareHitbox(bottomLeft,topRight));
	if(path!=NULL){
		if(!sprite.load(path,tileWidth,tileHeight,tileIndices,tileRotations,width,height)){
			std::cout << "Error : texture file " <<path<<" not found\n";
		}
		sprite.setPosition(bottomLeft.x,bottomLeft.y);
		sprite.setScale((topRight.x-bottomLeft.x)/(width*tileWidth),(topRight.y-bottomLeft.y)/(height*tileHeight));
	}	
}

void Still::draw(sf::RenderWindow* window)const{
	if(!sprite.isLoaded()){
		for(unsigned int i=0;i<hitboxes.size();i++){
			hitboxes[i]->draw(window,sf::Color(255,255,255));
		}
	}else{
		window->draw(sprite);
	}
}

bool Still::loadFromFile(FILE* f, Room* r){
	if (f == NULL)
		return false;

	char texturePath [200];
	int tileHeight=1;
	int tileWidth=1;
	int tileSetHeight=1;
	int tileSetWidth=1;
	std::vector<int> tileIndices;
	std::vector<int> tileRotations;
	Point2d origin;
	double scale = 1.0;

	//have a temporary buffer used to read the file line by line...
	char buffer[1000];
	//this is where it happens.
	while (!feof(f)){
		//get a line from the file...
		Game::readValidLine(buffer, f, sizeof(buffer));
		char *line = Game::lTrim(buffer);

		int lineType = Game::getTypeID(line);
		switch (lineType) {
			case SQUARE_HITBOX:{
					SquareHitbox* box = new SquareHitbox();
					if (sscanf(line, "%lf %lf %lf %lf", &(box->min.x), &(box->min.y), &(box->max.x), &(box->max.y)) != 4)
						return false;
					hitboxes.push_back(box);
				}break;
			case CONVEX_POLYGON:{
					ConvexPolygon* polygon = new ConvexPolygon();
					int nPoints;
					int currentOffset;
					int offset=0;
					if(sscanf(line,"%d %n",&nPoints,&offset)!=2)
						return false;
					polygon->vertices.resize(nPoints);
					for(int i=0;i<nPoints;i++){
						if(sscanf(line+offset, "%lf %lf %n", &(polygon->vertices[i].x),&(polygon->vertices[i].x),&currentOffset) != 2)
							return false;
						offset+=currentOffset;
					}
					hitboxes.push_back(polygon);
				}break;
			case TEXTURE_PATH:{
					strcpy(texturePath,Game::trim(line));
				}break;
			case TILE_DIMENSION:{
					if(sscanf(line,"%d %d",&tileWidth,&tileHeight)!=2)
						return false;
				}break;
			case TILE_INDICES:{
					int size = tileSetWidth*tileSetHeight;
					tileIndices.resize(size);
					int currentOffset=0;
					int offset=0;
					for(int i=0;i<size;i++){
						offset+=currentOffset;
						if(sscanf(line+offset, "%d %n", &tileIndices[i],&currentOffset) != 1)
							return false;
					}
				}break;
			case TILE_ROTATIONS:{
					int size = tileSetWidth*tileSetHeight;
					tileRotations.resize(size);
					int currentOffset=0;
					int offset=0;
					for(int i=0;i<size;i++){
						offset+=currentOffset;
						if(sscanf(line+offset, "%d %n", &tileRotations[i],&currentOffset) != 1)
							return false;
					}
				}break;
			case TILESET_DIMENSION:{
					if(sscanf(line,"%d %d",&tileSetWidth,&tileSetHeight)!=2)
						return false;
				}break;
			case TILE_ORIGIN:{
					if(sscanf(line,"%lf %lf",&origin.x,&origin.y)!=2)
						return false;
				}break;
			case TILE_SCALE:{
					if(sscanf(line,"%lf",&scale)!=1)
						return false;
				}break;
			case END:
				if(texturePath!=NULL)
					if(!sprite.load(texturePath,tileWidth,tileHeight,tileIndices,tileRotations,tileSetWidth,tileSetHeight))
						std::cout << "Error : texture file " <<texturePath<<" not found\n";
				sprite.setPosition(origin.x,origin.y);
				sprite.setScale(scale,scale);	
				return true;
				break;
			default:
				return false;
		}
	}
	return false;
}