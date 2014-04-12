#include "TileMap.h"

bool TileMap::load(const char* texturePath, int tileWidth, int tileHeight, std::vector<int> &tileIndices, std::vector<int> &tileRotations, int width, int height){
	if (!m_tileset.loadFromFile(texturePath))
		return false;

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	for(unsigned int i=0;i<width;i++){
		for(unsigned int j=0;j<height;j++){
			int tileIndex = tileIndices[i+j*width];
			if(tileIndex!=-1){
				int tx = tileIndex;
				int ty = 0.0;
				sf::Vertex* quad=&m_vertices[(i+j*width)*4];
				quad[0].position=sf::Vector2f(i*tileWidth,j*tileHeight);
				quad[1].position=sf::Vector2f((i+1)*tileWidth,j*tileHeight);
				quad[2].position=sf::Vector2f((i+1)*tileWidth,(j+1)*tileHeight);
				quad[3].position=sf::Vector2f(i*tileWidth,(j+1)*tileHeight);
				quad[(4-tileRotations[tileIndex])%4].texCoords=sf::Vector2f(tx*tileWidth, ty*tileHeight);
				quad[(5-tileRotations[tileIndex])%4].texCoords = sf::Vector2f((tx+1)*tileWidth,ty*tileHeight);
				quad[(6-tileRotations[tileIndex])%4].texCoords = sf::Vector2f((tx+1)*tileWidth,(ty+1)*tileHeight);
				quad[(7-tileRotations[tileIndex])%4].texCoords = sf::Vector2f(tx*tileWidth,(ty+1)*tileHeight);
			}
		}
	}
	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}