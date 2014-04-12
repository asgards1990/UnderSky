#pragma once

#include "Object.h"
#include "TileMap.h"

class Hitbox;

/*
a static object in the scene
*/
class Still : public Object{
public:
	/*
	path : path of the texture file
	tileHeight : height in pixel of one tile
	tileWidth : width in pixel of one tile
	tileIndices : matrix mapping squares to tiles. -1 stands for an empty square, positive values for indices of tiles in the texture
	tileRotations : matrix of rotations of each square. 0 : 0°, 1 : 90°, 2 : 180°, 3 : 270°
	width : number of tiles along the x axis
	height : number of tiles along the y axis
	origin : center of gravity in world coordinates
	scale : scaling factor from pixel coordinates to world coordinates
	*/
	Still(std::vector<Hitbox*> &p_hitboxes = std::vector<Hitbox*>(), char* path = NULL, int tileHeight = 0, int tileWidth = 0, std::vector<int> &tileIndices = std::vector<int>(), std::vector<int> &tileRotations = std::vector<int>(), int width = 0, int height = 0, Point2d origin = Point2d(), double scale = 1.0);
	/*
	bottomLeft : world coordinates of the bottom left corner of the still
	topRight : world coordinates of the top right corner of the still
	*/
	Still(Point2d bottomLeft, Point2d topRight, char* path = NULL, int tileHeight = 0, int tileWidth = 0, std::vector<int> &tileIndices = std::vector<int>(), std::vector<int> &tileRotations = std::vector<int>(), int width = 0, int height = 0);
	virtual ~Still(void);

	virtual void draw(sf::RenderWindow* window)const;

	bool loadFromFile(FILE* f, Room* r);

protected:
	TileMap sprite;
};

